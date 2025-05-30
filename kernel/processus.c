#include <inttypes.h>


#include "n7OS/processus.h"
#include <stdlib.h>
#include <n7OS/cpu.h>

#include "n7OS/fifo.h"

extern void ctx_sw(void *ctx_old, void *ctx_new); // Déclaration de la fonction ctx_sw
extern void idle(); // Déclaration de la fonction idle

void fun_envelope(void (*fun)(void), pid_t pid);


int nb_processus = 0; // Nombre de processus créés
int nb_processus_actifs = 0; // Nombre de processus actifs
int next_pid = 1; // Prochain PID à attribuer
pid_t processus_courant = 0;

process_t processus_table[NB_MAX_PROCESSUS]; // Table des processus

// PID du processus en cours

QUEUE *queue; // File d'attente pour les processus prêts

/*
    * @brief Initialise le système de gestion des processus
    * 
    * Cette fonction initialise la table des processus, la file d'attente et le processus idle.
    * Elle est appelée au démarrage du système pour préparer l'environnement d'exécution des processus.
*/
void init_processus() {
     
    cli(); // Désactiver les interruptions
    queue = (QUEUE *)malloc(sizeof(QUEUE)); // Allouer de la mémoire pour la file d'attente
    init_queue(queue); // Initialiser la file d'attente
    for (int i = 0; i < NB_MAX_PROCESSUS; i++) {
        processus_table[i].state = TERMINER; // Initialiser tous les processus comme bloqués et suspendus
        processus_table[i].pid = i; // PID invalide
    }

    processus_table[0].name = "idle_kernel"; // Nom du processus courant
    processus_table[0].pid = 0; // PID du processus courant
    processus_table[0].regs[0] = 0; // Adresse de la fonction
    processus_table[0].regs[1] = (uint32_t)&processus_table[0].stack[STACK_SIZE - 4]; // Initialiser la pile
    processus_table[0].regs[2] = 0; // Initialiser le registre EBP
    processus_table[0].regs[3] = 0; // Initialiser le registre EIP  
    processus_table[0].regs[4] = 0; // Initialiser le registre EFLAGS
    processus_table[0].stack[STACK_SIZE - 4] = (uint32_t)fun_envelope;
    processus_table[0].stack[STACK_SIZE - 2] = (uint32_t)idle; // Adresse de la fonction
    processus_table[0].stack[STACK_SIZE - 1] = (uint32_t)0; // PID du processus
    processus_table[0].state = BLOQUE; // État du processus courant
    processus_courant=0;
    //enqueue(queue, 0); // Ajouter le processus courant à la file d'attente

    sti(); // Réactiver les interruptions
}

/*
    * @brief Alloue un PID pour un nouveau processus
    * 
    * Cette fonction parcourt la table des processus pour trouver un PID disponible.
    * Si un PID est trouvé, il est marqué comme actif et retourné.
    * 
    * @return Le PID alloué ou -1 si aucun PID n'est disponible
*/
uint32_t alloc_pid() {
    for(int i = 1; i < NB_MAX_PROCESSUS; i++) {
        if (processus_table[i].state == TERMINER) {
            processus_table[i].state = ACTIF; // Marquer le processus comme prêt
            return i;
        }
    }
    return -1; // Aucun PID disponible
}

/*
    * @brief Crée un nouveau processus
    * 
    * Cette fonction alloue un PID, initialise les registres et la pile du processus,
    * et ajoute le processus à la file d'attente. Elle utilise une enveloppe pour exécuter
    * la fonction du processus.
    * 
    * @param name Le nom du nouveau processus
    * @param f La fonction que le nouveau processus doit exécuter
    * @return Le PID du nouveau processus ou -1 en cas d'échec
*/
pid_t creer_processus(const char *name, fnptr f) {

    //cli(); // Désactiver les interruptions

    pid_t pid = alloc_pid();
    if (pid == -1) return -1; // Aucun PID disponible

    processus_table[pid].name = name; // Nom du processus
    processus_table[pid].pid = pid; // PID commence à 1
    processus_table[pid].regs[0] = 0; // Adresse de la fonction
    processus_table[pid].regs[1] = (uint32_t)&processus_table[pid].stack[STACK_SIZE - 4]; // Initialiser la pile
    processus_table[pid].regs[2] = 0; // Initialiser le registre EBP
    processus_table[pid].regs[3] = 0; // Initialiser le registre EIP
    processus_table[pid].regs[4] = 0; // Initialiser le registre EFLAGS
    processus_table[pid].stack[STACK_SIZE - 4] = (uint32_t)fun_envelope;
    processus_table[pid].stack[STACK_SIZE - 2] = (uint32_t)f; // Adresse de la fonction
    processus_table[pid].stack[STACK_SIZE - 1] = (uint32_t)pid; // PID du processus
    processus_table[pid].state = ACTIF; // État du processus    
    nb_processus++;
    nb_processus_actifs++;

    // Ajouter le processus à la file d'attente
    if (!is_full(queue)) {
        enqueue(queue, pid);
    } else {
        // Gérer le cas où la file d'attente est pleine
        return -1; // File d'attente pleine
    }
    //sti(); // Réactiver les interruptions 

    return processus_table[pid].pid;
   
}
    
/*
    * @brief Termine un processus
    * 
    * Cette fonction marque un processus comme terminé, met à jour le nombre de processus actifs,
    * et retire le processus de la file d'attente. Elle appelle également le planificateur pour
    * gérer le changement de contexte.
    * 
    * @param pid Le PID du processus à terminer
*/
void terminer_processus(pid_t pid) {
    cli(); // Désactiver les interruptions
    if (pid == 0 || pid > NB_MAX_PROCESSUS) return -1; // PID invalide
    process_t *proc = &processus_table[pid]; //a verifier le -1
    if (proc->state == TERMINER) return -1; // Processus déjà terminé
    proc->state = TERMINER;
    nb_processus--;
    // Retirer le processus de la file d'attente
    remove_from_queue(queue, pid); // Nouvelle fonction à implémenter
    sti(); // Réactiver les interruptions
    //scheduler(); // Appeler le planificateur
}

/*
    * @brief Bloque un processus
    * 
    * Cette fonction marque un processus comme bloqué, met à jour le nombre de processus actifs,
    * et retire le processus de la file d'attente. Elle appelle également le planificateur pour
    * gérer le changement de contexte.
    * 
    * @param pid Le PID du processus à bloquer
*/
void bloquer_processus(pid_t pid) {
    if (pid == 0 || pid > NB_MAX_PROCESSUS) return; // PID invalide
    process_t *proc = &processus_table[pid];
    if (proc->state == ACTIF) {
        proc->state = BLOQUE;
        nb_processus_actifs--;
        //Retirer le processus de la file d'attente
        //dequeue(queue);
    }

    //scheduler(); // Appeler le planificateur
}


/*
    * @brief Débloque un processus
    * 
    * Cette fonction marque un processus comme actif, met à jour le nombre de processus actifs,
    * et ajoute le processus à la file d'attente. Elle appelle également le planificateur pour
    * gérer le changement de contexte.
    * 
    * @param pid Le PID du processus à débloquer
*/

/*
 Le processus idle_kernel n'est jamais dans la file d'attente. 
 il laisse les autres processus s'executer.
 et quand il n'y a plus de processus, il s'exécute.
 il est toujours actif.

 on change de contexte que s'il y a plusieurs processus dans la file. 
*/
void scheduler() {
    cli(); // Désactiver les interruptions

    pid_t suivant;

    uint32_t ctx_old = processus_courant; // Sauvegarder le contexte du processus courant
    suivant = dequeue(queue); // Retirer le processus de la file d'attente

    if(processus_table[ctx_old].state == ELU){
        // Si le processus courant est élu, on le remet dans la file d'attente
        enqueue(queue, ctx_old);
        processus_table[ctx_old].state = ACTIF; // Marquer le processus courant comme prêt
    }

    if(suivant !=0){
        processus_table[suivant].state = ELU; // Marquer le processus suivant comme élu
        processus_courant= suivant; // Mettre à jour le PID du processus courant

    }else{
        if(processus_table[ctx_old].state == TERMINER){
            processus_courant = 0; // Si le processus courant est terminé, on reste sur le processus idle
        }
    }

    sti(); // Réactiver les interruptions

    if(ctx_old != processus_courant) {
        //changer de contexte
        //printf("Changement de contexte du processus %d vers le processus %d\n", ctx_old, processus_courant);
        ctx_sw(processus_table[ctx_old].regs, processus_table[processus_courant].regs); // Changer de contexte
    }
}



void fun_envelope(void (*fun)(void), pid_t pid) {
    //printf("Processus %d en cours d'exécution\n", pid);
    // Appeler la fonction du processus
    fun();
    //printf("Processus %d terminé\n", pid);
    // Terminer le processus après l'exécution de la fonction
    //processus_table[pid].state = TERMINER; // Marquer le processus comme bloqué et suspendu
    //terminer_processus(pid); // Terminer le processus
    //scheduler(); // Appeler le planificateur

}

pid_t getpidCourant() {
    return processus_courant; // Retourner le PID du processus courant
}


#include <n7OS/console.h>
#include <n7OS/clavier.h>
#include <n7OS/processus.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <string.h>
#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <stdio.h>
#include <n7OS/time.h>
#include <unistd.h>
#include <n7OS/sys.h>
#include <n7OS/clavier.h>
#include <n7OS/irq.h>
#include <n7OS/processus.h>

#include <unistd.h>

#include "n7OS/minishell.h"

extern void processus1();
extern void idle();
extern void processus2();

static char cmd_buffer[MAX_CMD_LEN];
static int cmd_pos = 0;

extern process_t processus_table[NB_MAX_PROCESSUS]; // Table des processus

#define MAX_COMMANDS 10

// Modification de la déclaration du tableau
static shell_cmd_t *shell_cmds = NULL;
static int num_commands = 0;

// Prototypes des commandes internes
void cmd_help(void);
void cmd_ps(void);
void cmd_clear(void);
void cmd_shutdown(void);
void test(void);
void test_basculement_process();
void afficher_mem(void);

// Fonction pour ajouter une commande
static void add_command(const char *name, void (*func)(void), const char *help) {
    if (num_commands >= MAX_COMMANDS) {
        printf("ERREUR: Nombre maximum de commandes atteint\n");
        return;
    }

    shell_cmds[num_commands].name = name;
    shell_cmds[num_commands].func = func;
    shell_cmds[num_commands].help = help;
    num_commands++;
}

void init_shell(void) {
    // Allouer l'espace pour le tableau de commandes
    shell_cmds = (shell_cmd_t *)malloc(sizeof(shell_cmd_t) * MAX_COMMANDS);
    if (shell_cmds == NULL) {
        printf("ERREUR: Impossible d'allouer la memoire pour les commandes\n");
        return;
    }

    // Initialiser toutes les entrées à NULL
    for (int i = 0; i < MAX_COMMANDS; i++) {
        shell_cmds[i].name = NULL;
        shell_cmds[i].func = NULL;
        shell_cmds[i].help = NULL;
    }

    // Ajouter les commandes une par une
    add_command("help", cmd_help, "Affiche l'aide");
    add_command("ps", cmd_ps, "Liste les processus");
    add_command("clear", cmd_clear, "Efface l'ecran");
    add_command("shutdown", cmd_shutdown, "Arrete le systeme");
    add_command("test", test, "Test des fonctionnalites du projet");
    add_command("process", test_basculement_process, "Test de basculement de processus");
    add_command("mem", afficher_mem, "Affiche la memoire physique");
}


// Implémentation des commandes
void cmd_help(void) {
    printf("Commandes disponibles:\n");
    for (const shell_cmd_t *cmd = shell_cmds; cmd->name != NULL; cmd++) {
        printf("  %s: %s\n", cmd->name, cmd->help);
    }
    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void cmd_ps(void) {
    printf("PID\tETAT\t NOM\n");
    printf("-------------------------\n");
    for (int i = 0; i < NB_MAX_PROCESSUS; i++) {
        if (processus_table[i].state != TERMINER) {
            printf("%3d\tACTIF\t%s\n", i, processus_table[i].name);
        }
    }

    //petite attente pour attendre la fin des tests sur plusieurs processus
    for(int i = 0; i < 100000000; i++) {
        // Simuler un travail
    }

    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void cmd_clear(void) {
    vider_ecran();
    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void cmd_shutdown(void) {
    printf("Arrêt du systeme...\n");
    shutdown(1); // Arrêt de QEMU
}

// Analyse et exécution des commandes
void execute_command(char *input_cmd) {
    // Vérification de l'initialisation du tableau de commandes
    if (shell_cmds[0].name == NULL) {
        printf(">> ERREUR : shell_cmds est vide ou non initialise !\n");
        return;
    }

    // Ignorer les commandes vides
    if (input_cmd[0] == '\0') return;

    // Chercher la commande dans la table des commandes internes
    for (int i = 0; shell_cmds[i].name != NULL; i++) {
        if (strcmp(input_cmd, shell_cmds[i].name) == 0) {
            myfork(shell_cmds[i].name, shell_cmds[i].func); // Exécuter la fonction associée
            return;
        }
    }

    printf("Commande inconnue: %s\n", input_cmd);
}

void test(){

    printf("------Test des fonctionnalites du projet--------\n");

    /*
    * Test de la paginationTest de la pagination
    */
    printf("------Test de la pagination--------\n");

    alloc_page_entry(0xA0000000, 0, 0);
    uint32_t *ptr= (uint32_t *)0xA0000000;
    uint32_t do_page_fault= *ptr;
    

    printf("    Alloc page entry : %x\n", do_page_fault);
    do_page_fault++;
    printf("    Page bien alloue : %x\n", do_page_fault);

    //-----------------------------------------------------//
    
    printf("\n");
    
    /*
     Test de l'appel système
     */
    printf("------Test de l'appel systeme--------\n");
    printf("Appel systeme shutdown avec 4 au lieu de 1 : %d\n",shutdown(4));
    printf("Tester commande shutdown pour fermer\n");
    
    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void test_basculement_process() {
    printf("------Test de basculement de processus--------\n");
    
    // Créer un processus pour tester le basculement
    myfork("processus1", processus1);
    myfork("processus2", processus2);

    for(int i = 0; i < 1000000; i++) {
    }

    cmd_ps(); // Afficher les processus actifs

    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void afficher_mem(void) {
    printf("------Affichage de la memoire physique--------\n");
    
    print_mem(); // Afficher l'état de la mémoire physique

    printf(SHELL_PROMPT);
    myexit(); // Terminer le processus courant
}

void trim_whitespace(char *str) {
    // Enlever les espaces en fin de chaîne
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\r' || str[len - 1] == '\n'))
        str[--len] = '\0';

    // Enlever les espaces au début
    int start = 0;
    while (str[start] == ' ') start++;

    if (start > 0) {
        memmove(str, str + start, strlen(str + start) + 1);
    }
}

// Boucle principale du shell
void shell_run(void) {
    init_shell(); // Initialiser le shell
    printf(SHELL_PROMPT);
    while (1) {
        cmd_pos = 0;
        
        // Lecture de la commande
        while (1) {
            
            char c = kgetch();
            
            if (c == '\n') {
                printf("\n");
                cmd_buffer[cmd_pos] = '\0';
                break;
            }
            else if (c == '\b' && cmd_pos > 0) {
                cmd_pos--;
                effacer();
            }
            else if (c >= ' ' && cmd_pos < MAX_CMD_LEN - 1) {
                cmd_buffer[cmd_pos++] = c;
                console_putchar(c);
            }
        }

        trim_whitespace(cmd_buffer); // Enlever les espaces superflus
        // Exécuter la commande
        execute_command(cmd_buffer);
        hlt(); // Attendre la fin du processus
    }
}
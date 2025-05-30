#include <n7OS/time.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <n7OS/processus.h>


static ticks = 0; // Variable pour stocker le nombre de ticks // Déclaration de la fonction handler_en_C
extern void (*handlers_IT[])(void); // Déclaration de la fonction handler_en_C

/*
    * @brief Initialise l'horloge système
    * 
    * Cette fonction configure le Programmable Interval Timer (PIT) pour générer des interruptions à une fréquence donnée.
    * Elle initialise également l'entrée d'interruption correspondante dans l'IDT.
*/

void time_init() {
    // Initialisation de l'horloge système
    uint32_t freq = FREQ / FREQ_HZ; // Calcul de la valeur à envoyer au PIT
    uint32_t lsb = freq & 0xFF; // Partie basse
    uint32_t msb = (freq >> 8) & 0xFF; // Partie haute

    //masquage de l'IRQ0
    outb(0x36,0x43);

    // Sélection du canal 0
    outb(lsb, PIT_CHANNEL0); // Envoi de la partie basse
    outb(msb, PIT_CHANNEL0); // Envoi de la partie haute

    /* init_irq_entry(32, (uint32_t)handlers_IT[32-15]); 
    deja fait avec init_IRQ dans test_irq.c */

 // Initialisation de l'IDT et des entrées d'interruption

   //demasquage de l'IT
   outb(inb(0x21)&0xfe, 0x21); // Démasquer l'IRQ0
    
}


// Fonction de gestion de l'interruption du timer
void timer_handler_C() {

    cli(); // Désactiver les interruptions pour éviter les conflits
    

    //ack du handler
    // Envoyer un signal d'acknowledge à l'IRQ0
    // Incrémenter le compteur de ticks
   
    outb(0x20, 0x20); // Envoyer un signal d'acknowledge à l'IRQ0

    if(ticks % 1000 == 0) { // Chaque seconde
        // Vous pouvez ajouter ici le code à exécuter chaque seconde
        // Par exemple, mettre à jour l'interface utilisateur ou effectuer d'autres tâches périodiques
        time_get_hms();
    }
    

    ticks++;

    sti(); // Réactiver les interruptions
    if(ticks % 10 == 0) { 
        scheduler(); // Appeler le planificateur pour les processus
         // Réactiver les interruptions
    }

    
}

// Fonction pour obtenir l'heure actuelle au format HH:MM:SS
void time_get_hms() {
    uint32_t hours, minutes, seconds;
    seconds = (ticks / 1000) % 60;
    minutes = (ticks / (1000 * 60)) % 60;
    hours = (ticks / (1000 * 3600)) % 24;
    int ligne = get_ligne();
    int colonne = get_colonne();
    position_cursor(0,72,1);
    printf("%02u:%02u:%02u\n", hours, minutes, seconds);
    position_cursor(ligne, colonne,0);
}
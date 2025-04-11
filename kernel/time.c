#include <n7OS/time.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>


static ticks = 0; // Variable pour stocker le nombre de ticks // Déclaration de la fonction handler_en_C
extern void (*handlers_IT[])(void); // Déclaration de la fonction handler_en_C


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

    init_irq_entry(32, (uint32_t)handlers_IT[32-15]); // Initialisation de l'entrée d'interruption pour le timer

    //init_irq_entry(32, (uint32_t)timer_handler); // Initialisation de l'entrée d'interruption pour le timer

 // Initialisation de l'IDT et des entrées d'interruption

   //demasquage de l'IT
   outb(inb(0x21)&0xfe, 0x21); // Démasquer l'IRQ0
    
}

void timer_handler_C() {

    
    

    //ack du handler
     // Envoyer un signal d'acknowledge à l'IRQ0
    // Incrémenter le compteur de ticks
   
    outb(0x20, 0x20); // Envoyer un signal d'acknowledge à l'IRQ0

    if(ticks % 1000 == 0) { // Chaque seconde
        //printf("Tick: %u\n", ticks);
        // Vous pouvez ajouter ici le code à exécuter chaque seconde
        // Par exemple, mettre à jour l'interface utilisateur ou effectuer d'autres tâches périodiques
        time_get_hms();
    }
    

    ticks++;
    
        
        // Chaque seconde
        // Vous pouvez ajouter ici le code à exécuter chaque seconde
        // Par exemple, mettre à jour l'interface utilisateur ou effectuer d'autres tâches périodiques
        
    

    
    // Code pour gérer l'interruption du timer
    // Par exemple, vous pouvez appeler une fonction pour mettre à jour l'interface utilisateur
    // ou effectuer d'autres tâches périodiques.

    
}

void time_get_hms() {
    uint32_t hours, minutes, seconds;
    seconds = (ticks / 1000) % 60;
    minutes = (ticks / (1000 * 60)) % 60;
    hours = (ticks / (1000 * 3600)) % 24;
    position_cursor(0,72);
    printf("%02u:%02u:%02u\n", hours, minutes, seconds);
}
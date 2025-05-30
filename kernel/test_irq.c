#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <n7OS/time.h>
#include <n7OS/clavier.h>

//extern void init_irq();
extern void (*handlers_IT[])(void);




/*
    * @brief Initialise l'IDT et les entrées d'interruption
    * 
    * @param irq_num Numéro de l'interruption
    * @param handler Adresse du gestionnaire d'interruption
*/


void init_irq() {
    // Set up the handler for interrupt 50 (IRQ0)
    //Set up the handler for interrupt 50 to 60 (IRQ0)
    // utilisation d'un tableau de handlers
    // initialise tous les gestionnaires d'interruption de 15 à 60
    for (int i = 15; i <= 60; i++) {
        init_irq_entry(i, (uint32_t)handlers_IT[i - 15]);
    }
    
}



void handler_en_C(int irq_num) {
    
    // Do something
    // Clear the interrupt flag
    switch (irq_num)
    {
    case 32: 
        timer_handler_C();
        break;
    
    case 33:
        keyboard_handler_C();
        break;
    
    default: printf("Handler for IRQ %d\n", irq_num);
        break;
    }

}


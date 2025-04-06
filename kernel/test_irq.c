#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

//extern void init_irq();
extern void (*handlers_IT[])(void);

void init_irq_entry(int irq_num, uint32_t handler);



/*
    * @brief Initialise l'IDT et les entrées d'interruption
    * 
    * @param irq_num Numéro de l'interruption
    * @param handler Adresse du gestionnaire d'interruption
*/


void init_irq() {
    

    // Set up the handler for interrupt 50 (IRQ0)
    //Set up the handler for interrupt 50 to 60 (IRQ0)
    for (int i = 14; i <= 60; i++) {
        init_irq_entry(i, (uint32_t)handlers_IT[i - 14]);
    }
    
}


void handler_en_C(int irq_num) {
    
    // Do something
    // Clear the interrupt flag
    switch (irq_num)
    {
    case 14 : printf("Handler for segmentation fault\n");
        while (1)
        {
            hlt();
        }
        break;
    
    default: printf("Handler for IRQ %d\n", irq_num);
        break;
    }

}

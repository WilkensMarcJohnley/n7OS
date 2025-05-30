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

extern void processus1();
extern void idle();
extern void processus2();

void kernel_start(void)
{
    uint32_t page_dir;
    init_console();
    //setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    kmalloc_init();
    page_dir=initialise_paging();
    setup_base(page_dir);

    // lancement des interruptions
    init_irq();
    init_syscall();

    time_init();

    init_keyboard();



    sti();
    
    // tout les tests dans le mini shell


    /*
       test de la pagination
    */


   

    /*
     ---------------------------------------------------------------------------
     ----------------- fin test de la pagination -----------------
     ---------------------------------------------------------------------------
    */


    

    /*
     test d'une interruption
    */

    
   /*  __asm__ ("int $(32)");
    __asm__("int $(50)");
    __asm__("int $(14)");
    __asm__ ("int $(50)");
    __asm__("int $(20)"); */

    //__asm__("int $(33)");


    /*
       fin d'un test d'interruption    
    */

    /*
        test de l'appel systeme
    */ 
    
   /*  if(example() == 1)
        printf("Appel systeme reussi\n"); */

    init_processus();
    
    //creer_processus("processus_idle", idle);
    //creer_processus("processus1", processus1);
    //creer_processus("processus_idle", idle);
    //idle();
    
    //scheduler();

    //myfork("processus1", processus1);
    //myfork("processus2", processus2);
    
    idle();


    

    //shutdown(1);

    /* 
        fin test de l'appel systeme
     */

     //print_mem();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur

        //recuperation et ecriture d'un caractere recu au clavier
        char c = kgetch();        // Attendre une touche
        console_putchar(c); 
        hlt();
    }
}

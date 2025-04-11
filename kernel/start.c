#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <stdio.h>
#include <n7OS/time.h>



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
    time_init();
    sti();
    
    


    /*
       test de la pagination
    */

    alloc_page_entry(0xA0000000, 0, 0);
    uint32_t *ptr= (uint32_t *)0xA0000000;
    uint32_t do_page_fault= *ptr;
    do_page_fault++;

    /*
     ---------------------------------------------------------------------------
     ----------------- fin test de la pagination -----------------
     ---------------------------------------------------------------------------
    */
    

    /*
     test d'une interruption
    */

    
    //__asm__ ("int $(32)");
    //__asm__("int $(50)");
    //__asm__("int $(14)");
    //__asm__ ("int $(50)");
    //__asm__("int $(20)");
    /*
       fin d'un test d'interruption    
    */
    


    //effacer();
   
    
    printf("Paging initialized\n");
     //print_mem();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

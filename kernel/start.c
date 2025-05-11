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

    /*
        test de l'interruption timer 
     */

    time_init();

    /*
        fin test interruprion timer 
     */

    /*
        test de l'interruption clavier
    */
    init_keyboard();


    sti();
    
    


    /*
       test de la pagination
    */

    

    alloc_page_entry(0xA0000000, 0, 0);
    uint32_t *ptr= (uint32_t *)0xA0000000;
    uint32_t do_page_fault= *ptr;
    printf("test de la pagination : %x\n", do_page_fault);
    do_page_fault++;

    printf("test de la pagination reussi : %x\n", do_page_fault);

    /*
     ---------------------------------------------------------------------------
     ----------------- fin test de la pagination -----------------
     ---------------------------------------------------------------------------
    */

    test_clavier();
    

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
    
    if(example() == 1)
        printf("Appel systeme reussi\n");

     printf("appel systeme shutdown : %d\n",shutdown(4));

     //shutdown(1);

    /* 
        fin test de l'appel systeme
     */

     //print_mem();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

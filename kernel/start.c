#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <stdio.h>

void kernel_start(void)
{
    uint32_t page_dir;
    init_console();
    //setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    kmalloc_init();
    page_dir=initialise_paging();
    setup_base(page_dir);
    
    // lancement des interruptions
    sti();


    //effacer();
   
    
    printf("Paging initialized\n");
    print_mem();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

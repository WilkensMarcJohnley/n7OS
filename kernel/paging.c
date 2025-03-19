#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/mem.h>
#include <n7OS/cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PageDirectory page_dir;
//PageTable *page_table;

void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
    page_table_entry->page_entry.p= 1;
    page_table_entry->page_entry.a= 0;
    page_table_entry->page_entry.d= 0;
    page_table_entry->page_entry.w= is_writeable;
    page_table_entry->page_entry.u= ~is_kernel;
    page_table_entry->page_entry.page= new_page>>12;
}


uint32_t initialise_paging() {
    uint32_t index =0;
    init_mem();
    page_dir= (PageDirectory)kmalloc_a(sizeof(PDE)*1024);
    memset(page_dir, 0, sizeof(PDE)*1024);

    for(int i=0; i<1024; i++){
        PageTable new_pagetable= (PageTable)kmalloc_a(sizeof(PTE)*1024);
        memset(new_pagetable, 0, sizeof(PTE)*1024);
        page_dir[i].value= (uint32_t) new_pagetable | PAGE_PRESENT |PAGE_RW;
        index= (uint32_t) new_pagetable;
    }

    for(int i=0; i<index; i+=PAGE_SIZE){
        alloc_page_entry(i, 1, 1);
    }

    //register_interrupt_handler(14, handler_page_fault);

    
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(page_dir));

    //enabling paging
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));

    uint32_t testcr0, testcr3;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(testcr0));
    __asm__ __volatile__("mov %%cr3, %0" : "=r"(testcr3));
    printf("CR3 : %x\n", testcr3);
    printf("CR0 : %x\n", testcr0);
    printf("page_dir : %x\n", page_dir);

    return page_dir;
}


PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // address = adresse virtuelle à allouer 
    // address = idx_PDE | idx_PTE | offset
    //             10    |    10   |   12

    int idx_pagedir= address>>22; // calcul de la position de la table de page dans le répertoire de page
    PageTable page_table;
    
    PDE page_dir_entry= page_dir[idx_pagedir]; // on recupere l'entree dans le répertoire de page
    // une entree contient l'adresse de la table de page + bits de controle
    
    page_table= (PTE *) ((uint32_t)page_dir_entry.dir_entry.page << 12); // on recupere l'adresse de la page de table dans le répertoire de page
        
    uint32_t phy_page= findfreePage(); // recherche d'une page libre dans la memoire physique

    int idx_pagetab= 0x3FF & (address>>12); // calcul de la position de la page dans la table de page
   
    
    // mise a jour de l'entree dans la page de table
    setPageEntry(&page_table[idx_pagetab], phy_page, is_writeable, is_kernel);
    

    return page_table;

}

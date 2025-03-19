/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#define PAGE_PRESENT 1
#define PAGE_RW      2
#define PAGE_USER    4
#define PAGE_DIRTY   16

#include <inttypes.h>

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
    // a completer
    uint32_t p:1;
    uint32_t w:1;   
    uint32_t u:1;
    uint32_t a:1;
    uint32_t d:1;
    uint32_t unused:7;
    uint32_t page :20;
} page_table_entry_t;

/**
 * @brief Description d'une entree dans le repertoire de page
 */
typedef struct{
    uint32_t p:1;
    uint32_t w:1;
    uint32_t u:1;
    uint32_t rsvd:9;
    uint32_t page:20;
}page_directory_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;

typedef union {
    page_directory_entry_t dir_entry;
    uint32_t value;
} PDE; // PDE = Page Directory Entry

typedef PDE * PageDirectory;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
uint32_t initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);

#endif
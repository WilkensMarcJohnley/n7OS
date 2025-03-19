#include <n7OS/mem.h>

#define NBOCTETPAGES 32 //nb page representé par un entier


int size=PAGE_SIZE/32;
uint32_t free_page_bitmap_table[PAGE_SIZE/32];


/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t bit_pos= addr/PAGE_SIZE; //numero de la page
    free_page_bitmap_table[bit_pos/NBOCTETPAGES] |= 1 << (bit_pos%NBOCTETPAGES);
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    uint32_t bit_pos= addr/PAGE_SIZE;
    free_page_bitmap_table[bit_pos/NBOCTETPAGES] &= ~(1 << (bit_pos%NBOCTETPAGES));   
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t adresse= 0xFFFFFFFF;
    for(uint32_t i=0; i<size; i++) {
        if(free_page_bitmap_table[i]!=0xFFFFFFFF) {
            for(uint32_t j=0; j<NBOCTETPAGES; j++) {
                if(!(free_page_bitmap_table[i] & (1 << j))) {
                    //free_page_bitmap_table[i] |= 1 << j;
                    adresse=(i*NBOCTETPAGES+j)<<12;
                    setPage(adresse);
                    return adresse;
                }
            }
        }
    }
    return adresse;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    for(int i=0; i<size; i++) {
        free_page_bitmap_table[i]= 0x0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    /*for(uint32_t i=0; i<size; i++) {
        printf("%d ", free_page_bitmap_table[i/NBOCTETPAGES]);
        if(free_page_bitmap_table[i/NBOCTETPAGES] & (1 << (i%NBOCTETPAGES))) {
            printf("%d ", free_page_bitmap_table[i/NBOCTETPAGES]);
        } else {
            printf("0 ");
        }

        if(i%32==31) {
            printf("\n");
        }
    }*/

    for (int i = 0; i < size; i++) {
        uint32_t value = free_page_bitmap_table[i];
        printf("T %d: 0x%X ; ", i, value);
    }
}
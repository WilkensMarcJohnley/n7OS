#include <inttypes.h>
#include <n7OS/irq.h>
#include <n7OS/segment.h>
#include <n7OS/processor_structs.h>


/*
    * Une entrée dans l'IDT est sur 64 bits
    * 
    * Partie poids forts
    * Bit :     | 31              16 | 15 | 14 13 | 12 | 11 10 9 8 | 7 6 5 | 4 3 2 1 0 |
    * Contenu : | offset supérieur   | P  | DPL   | S  | GateType  | 0 0 0 |  réservé  |
    * 
    * Partie poids faible
    * Bit :     | 31              16 | 15              0 |
    * Contenu : |sélecteur de segment| offset inférieur  |
*/

void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t *idt_entry;
    idt_entry->offset_inf = addr & 0xFFFF;
    idt_entry->sel_segment = KERNEL_CS; // kernel code segment
    idt_entry->zero = 0;
    idt_entry->type_attr = 0x8E; // interrupt gate, present, DPL=0
    idt_entry->offset_sup = (addr >> 16) & 0xFFFF;


    idt[irq_num] = *((uint64_t*) idt_entry);
}

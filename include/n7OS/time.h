/*
    Module de gestion de l'horloge systeme a 1 kHz
    @author : Wilkens

*/

#ifndef _TIME_H
#define _TIME_H
#include <inttypes.h>
#include <n7OS/cpu.h>

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41 
#define PIT_CHANNEL2 0x42
#define FREQ 1190000 // fréquence de l'oscillateur
#define FREQ_HZ 1000 // fréquence de l'horloge système 
#define TIMER_IRQ 0x20 // IRQ pour le timer


void timer_handler();
void time_get_hms();
void time_init();


#endif
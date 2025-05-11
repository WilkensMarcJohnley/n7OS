/*
    Module de gestion du clavier
    @author : Wilkens
*/

#ifndef _CLAVIER_H
#define _CLAVIER_H

#define KEYBOARD_IRQ 0x21 // IRQ pour le clavier   
#define KEYBOARD_PORT 0x60 // Port d'entrée du clavier
#define KEYBOARD_COMMAND_PORT 0x64 // Port de commande du clavier
#define KEYBOARD_DATA_PORT 0x64 // Port d'état du clavier

#define BUFFER_SIZE 256 // Taille du tampon de clavier


void init_keyboard();

void keyboard_handler_C();

void test_clavier();


#endif
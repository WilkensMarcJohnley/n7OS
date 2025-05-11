#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <string.h>

void decaler_ligne();
void copier_ligne(int ligne, int ligne_dest);
void position_cursor(int ligne, int colonne);
void console_putchar(const char c);
void console_putbytes(const char *s, int len);
void effacer();
void test();
void vider_ecran();

uint16_t *scr_tab;
int ligne= 0; // Ligne courante
int colonne= 0; // Colonne courante
int cursor_pos; // Position du curseur

/** 
 * @brief Initialise la console
*/

void init_console() {
    
    scr_tab= (uint16_t *) SCREEN_ADDR;
    vider_ecran();
    position_cursor(ligne, colonne);   
}

void memset_16(uint16_t *dest, uint16_t value, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        dest[i] = value;
    }
}

void effacer_ligne(int ligne){
    memset_16(scr_tab+VGA_WIDTH*ligne, CHAR_COLOR<<8|' ', VGA_WIDTH*sizeof(uint16_t));
}
void vider_ecran(){
    memset_16(scr_tab, CHAR_COLOR<<8|' ', VGA_WIDTH*VGA_HEIGHT*sizeof(uint16_t));
    ligne= 0;
    colonne= 0;
    position_cursor(ligne, colonne);
}

/** 
 * @brief Positionne le curseur à la position (x, y)
 *
 * @param ligne: ligne
 * @param colonne: colonne 
*/

void position_cursor(int x, int y) {
    
    if(ligne==VGA_HEIGHT) {
        decaler_ligne();
        ligne= VGA_HEIGHT-1;
        colonne= 0;
    }

    ligne=x;
    colonne=y;

    if(ligne==0 && colonne==65) {
        ligne++;
        colonne= 0;
    }

    cursor_pos= VGA_WIDTH*ligne+colonne;
    outb(CMD_LOW, PORT_CMD);   
    outb(cursor_pos, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb((cursor_pos>>8), PORT_DATA);
}

int obtenir_curseur(){
    outb(CMD_LOW, PORT_CMD);
    int pos= inb(PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    pos|= inb(PORT_DATA)<<8;
    return pos;
}

void console_putchar(const char c) {
     
    if(c>31 && c<127) {
        scr_tab[cursor_pos]= CHAR_COLOR<<8|c;
        ligne+= (colonne==VGA_WIDTH-1)?1:0;
        colonne= (colonne==VGA_WIDTH-1)?0:colonne+1;
    }else if(c=='\n') {
        ligne++ ;
        colonne= 0;
    }else if(c=='\t') {
        if(colonne+4>79) {
            ligne++;
            colonne= 0;
        }else{
            colonne+= 4;
        }
    } else if(c=='\r'){
        colonne= 0;
    }else if(c=='\b') {
        if(ligne==0 && colonne==0) {
            return;
        }

        if(ligne==1 && colonne==0) {
            ligne= 0;
            colonne= 64;
        }else{
            if(ligne>0 && colonne==0) {
                ligne--;
                colonne= 79;
            }else if(colonne>0) {
                colonne--;
            }
        }
        
        cursor_pos= VGA_WIDTH*ligne+colonne;
        scr_tab[cursor_pos]= CHAR_COLOR<<8|' ';
    }else if(c=='\f') {
        vider_ecran();
        ligne= 0;
        colonne= 0;
    }
    position_cursor(ligne, colonne);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

/**
 * @brief Efface le caractère précédent
 */
void effacer(){
    console_putchar('\b');
    position_cursor(ligne, colonne);
    scr_tab[cursor_pos]= CHAR_COLOR<<8|' ';
}


/**
 * @brief copier une ligne dans une autre
 * @param ligne: ligne à copier
 * @param ligne_dest: ligne de destination
 */
void copier_ligne(int ligne, int ligne_dest){
    memcpy(scr_tab+VGA_WIDTH*ligne_dest, scr_tab+VGA_WIDTH*ligne, VGA_WIDTH*sizeof(uint16_t));
}

/**
 * @brief decaler les lignes vers le haut
 */
void decaler_ligne(){
    for(int i= 1; i<VGA_HEIGHT; i++) {
        copier_ligne(i, i-1);
    }
    effacer_ligne(VGA_HEIGHT-1);
    ligne= VGA_HEIGHT-1;
    colonne= 0;
    position_cursor(ligne, colonne);
}

int get_ligne(){
    return ligne;
}   

int get_colonne(){
    return colonne;
}


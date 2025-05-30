#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <string.h>

void decaler_ligne();
void copier_ligne(int ligne, int ligne_dest);
void position_cursor(int ligne, int colonne, int timer);
void console_putchar(const char c);
void console_putbytes(const char *s, int len);
void effacer();
void test();
void vider_ecran();
void remonter_ecran();

uint16_t *scr_tab;
int ligne= 0; // Ligne courante
int colonne= 0; // Colonne courante
int cursor_pos; // Position du curseur

/** 
 * @brief Initialise la console
 * efface l'ecran au premier lancement
 * et positionne le curseur a 0,0
*/

void init_console() {
    
    scr_tab= (uint16_t *) SCREEN_ADDR;
    vider_ecran();
    position_cursor(ligne, colonne,0);   
}

/*
    * @brief Remplit une zone mémoire avec une valeur spécifique
    * 
    * @param dest: Pointeur vers la zone mémoire à remplir
    * @param value: Valeur à utiliser pour remplir la zone mémoire
    * @param count: Nombre d'éléments à remplir
    * @note: Cette fonction a ete fait dans le but d'eviter des bugs
*/
void memset_16(uint16_t *dest, uint16_t value, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        dest[i] = value;
    }
}

/**
 * @brief Efface une ligne de la console
 * 
 * @param ligne: Ligne à effacer
 */
void effacer_ligne(int ligne){
    memset(scr_tab+ ligne * VGA_WIDTH, 0, sizeof(uint16_t));
}


/**
 * @brief Vide l'écran de la console
 * 
 * Cette fonction remplit l'écran avec des espaces et remet le curseur à la position (0, 0).
 */
void vider_ecran(){
    memset_16(scr_tab, CHAR_COLOR<<8|' ', VGA_WIDTH*VGA_HEIGHT*sizeof(uint16_t));
    ligne= 0;
    colonne= 0;
    position_cursor(ligne, colonne,0);
}

/** 
 * @brief Positionne le curseur à la position (x, y, timer)
 *
 * @param ligne: ligne
 * @param colonne: colonne 
 * @param timer : si c'est pour afficher le timer
 * Cette fonction met à jour la position du curseur dans la console 
 * selon qu'il doit afficher le temps ou non.
*/

void position_cursor(int x, int y, int timer) {
    
    if(ligne>=VGA_HEIGHT) {
        decaler();
        ligne= VGA_HEIGHT-1;
        colonne= 0;
    }

    // Si c'est pour afficher le timer, on ne change pas la ligne
    ligne=((ligne== VGA_HEIGHT -1) && (!timer))? ligne : x;
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

/**
 * @brief Affiche un caractère à la position actuelle du curseur
 * 
 * @param c: Caractère à afficher
 * 
 * Cette fonction gère l'affichage des caractères dans la console, y compris les caractères spéciaux
 * comme les retours à la ligne, les tabulations, et les effacements.
 */
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
    position_cursor(ligne, colonne,0);
}


/**
 * @brief Affiche une chaîne de caractères dans la console
 * 
 * @param s: Pointeur vers la chaîne de caractères à afficher
 * @param len: Longueur de la chaîne de caractères
 * 
 * Cette fonction parcourt la chaîne de caractères et appelle console_putchar pour chaque caractère.
 */
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
    position_cursor(ligne, colonne,0);
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



// renvoie la ligne actuelle du curseur
int get_ligne(){
    return ligne;
}   

//renvoie la colonne actuelle du curseur
int get_colonne(){
    return colonne;
}

/**
 * @brief Décale toutes les lignes de l'écran vers le haut
 * 
 * Cette fonction décale toutes les lignes de l'écran vers le haut, 
 * en supprimant la première ligne et en ajoutant une nouvelle ligne vide en bas.
 */
void decaler() {
    // Décaler toutes les lignes vers le haut
    memcpy(scr_tab,
           scr_tab + VGA_WIDTH,
           sizeof(uint16_t) * VGA_WIDTH * (VGA_HEIGHT - 1));

    // Effacer la dernière ligne avec des espaces et attribut 0x07
    uint16_t* last_line = scr_tab + (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (int i = 0; i < VGA_WIDTH; i++) {
        last_line[i] = CHAR_COLOR<<8 |' ';  // caractère ' ' avec attribut gris sur noir
    }
}
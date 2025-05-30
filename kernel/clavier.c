#include <n7OS/clavier.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <n7OS/cpu.h>

extern void (*handlers_IT[])(void);

int shift_pressed = 0; // Indicateur pour Shift pressé


char buffer[BUFFER_SIZE]; // Tampon de clavier
int buffer_head = 0; // Tête du tampon
int buffer_tail = 0; // Queue du tampon
int buffer_size = 0; // Taille du tampon

char scancode_to_ascii[128] = {
    0, 27, '&', 'é', '"', '\'', '(', '-', 'è', '_', // 0x00 - 0x09
    'ç', 'à', ')', '=', '\b', // Backspace
    '\t',                    // Tab
    'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '^', '$', '\n',         // Enter key
    0,                      // Left control
    'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
    'ù', '`', 0,            // Left shift
    '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':',
    '!', 0,                // Right shift
    '*',
    0,                     // Alt
    ' ',                   // Space bar
    // le reste (touches F1-F12, etc.) est laissé à 0
};

/*
    * @brief Initialise le clavier
    * 
    * Cette fonction configure le clavier pour recevoir les entrées et initialise le tampon de clavier.
    * Elle est appelée au démarrage du système pour préparer l'environnement d'entrée du clavier.
*/
void init_keyboard() {

    //init_irq_entry(33, (uint32_t)handlers_IT[33-15]);

      // Démasquage de l'IRQ1 (bit 1) dans le PIC maître
    uint8_t mask = inb(0x21);      // Lire le masque du PIC maître
    mask &= ~0x02;                 // Mettre le bit 1 (IRQ1) à 0 pour le démasquer
    outb(mask, 0x21); 
} 

/*
    * @brief Ajoute un caractère au tampon de clavier
    * 
    * Cette fonction ajoute un caractère au tampon de clavier si celui-ci n'est pas plein.
    * Elle gère le cas où le tampon est plein en ne faisant rien.
    * 
    * @param c Le caractère à ajouter au tampon
*/
void buffer_put(char c) {
    // Ajouter un caractère au tampon
    int next = (buffer_tail + 1) % BUFFER_SIZE;

    if (next != buffer_head) { // Vérifier si le tampon n'est pas plein
        buffer[buffer_tail] = c;
        buffer_tail = next;
        buffer_size++;
    }
}

// Fonction pour lire un caractère du tampon clavier
/*
    * @brief Lit un caractère du tampon de clavier
    * 
    * Cette fonction lit un caractère du tampon de clavier. Si le tampon est vide, elle attend
    * qu'un caractère soit disponible. Elle utilise une attente active (busy-wait) pour simplifier
    * la gestion des entrées.
    * 
    * @return Le caractère lu du tampon
*/
char kgetch() {
    // Attendre qu'un caractère soit disponible
    while (buffer_head == buffer_tail) {
        // Attente active (busy-wait) — peut être remplacée par une boucle plus propre
    }

    // Lire le caractère
    char c = buffer[buffer_head];

    // Avancer le pointeur de lecture
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    return c;
}

/*
    * @brief Gestionnaire d'interruption pour le clavier
    * 
    * Cette fonction est appelée lorsqu'une interruption du clavier se produit.
    * Elle lit le code de touche, le convertit en caractère ASCII, gère les touches Shift,
    * et ajoute le caractère au tampon de clavier. Elle envoie également un signal d'acknowledge
    * à l'IRQ1 pour indiquer que l'interruption a été traitée.
*/
void keyboard_handler_C() {
    // Gestionnaire d'interruption pour le clavier
    uint8_t scancode = inb(KEYBOARD_PORT); // Lire le code de touche

     // Détecter Shift pressé
     if (scancode == 0x2A || scancode == 0x36) {  // Shift Pressé
        shift_pressed = 1;
    } else if (scancode == 0xAA || scancode == 0xB6) { // Shift Relâché
        shift_pressed = 0;
    }

    char c = scancode_to_ascii[scancode]; // Convertir le code de touche en caractère ASCII

    if (shift_pressed) {
        // Convertir les lettres en majuscules
        if (c >= 'a' && c <= 'z') {
            c -= 32;  // Convertir en majuscule en utilisant la différence entre majuscule et minuscule dans la table ASCII
        }
        // Vérifier les symboles qui changent avec Shift
        else if (c == '1') c = '!';
        else if (c == '2') c = '"';
        else if (c == '3') c = '#';
        else if (c == '4') c = '$';
        else if (c == '5') c = '%';
        else if (c == '6') c = '^';
        else if (c == '7') c = '&';
        else if (c == '8') c = '\'';
        else if (c == '9') c = '(';
        else if (c == '0') c = ')';
        else if (c == '-') c = '_';
        else if (c == '=') c = '+';
        else if (c == '[') c = '{';
        else if (c == ']') c = '}';
        else if (c == '\\') c = '|';
        // Ajouter plus de symboles si nécessaire
    }

    // Ajouter le code de touche au tampon
    if (scancode & 0x80) { // Vérifier si c'est un code de relâchement
        // Ne rien faire pour les codes de relâchement
    } else {
        if (c != 0) {
            buffer_put(c); // Ajouter le caractère au tampon
        }
    
    }

    // Envoyer un signal d'acknowledge à l'IRQ1
    outb(0x20, 0x20); // Envoyer un signal d'acknowledge à l'IRQ1
}

/*
    * @brief Teste le clavier en affichant les caractères tapés
    * 
    * Cette fonction entre dans une boucle infinie, lisant les caractères du clavier et les affichant à l'écran.
    * Elle est utilisée pour tester le fonctionnement du clavier dans le système.
*/
void test_clavier() {
    while (1) {
        char c = kgetch();        // Attendre une touche
        console_putchar(c);         // L’afficher à l’écran
    }
}
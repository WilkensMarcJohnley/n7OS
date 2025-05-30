#include <n7OS/cpu.h>

void idle(){
    printf("Demarrage du mini shell ...\n");
    shell_run(); // Lancer le shell
}
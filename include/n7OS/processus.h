
#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <inttypes.h>

#define NB_MAX_PROCESSUS 255 // Nombre maximum de processus

#define STACK_SIZE 1024 // Taille de la pile d'un processus


typedef uint32_t pid_t; // Type pour identifier un processus


typedef enum {
    ELU, 
    ACTIF,
    BLOQUE,
    TERMINER
} PROCESS_STATE; // État d'un processus

typedef struct{
    uint32_t stack[STACK_SIZE]; // Pile du processus
    PROCESS_STATE state ;
    const char *name; // Nom du processus
    pid_t pid ;
    uint32_t regs[5]; // Registres du processus
} process_t;




typedef void* (*fnptr)();

//pid_t fork(const char *name, fnptr f);

//int exit();

pid_t getpid();

void init_processus();
pid_t creer_processus(const char *name, fnptr f);
void terminer_processus(pid_t pid);
void bloquer_processus(pid_t pid); 
void scheduler();
pid_t getpidCourant();



#endif
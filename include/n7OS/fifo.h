/* 
    Queue pour gerer la liste des processus prets 
 */

#ifndef __FIFO_H__
#define __FIFO_H__
#include <n7OS/processus.h>
#include <stdbool.h>


typedef struct {
    pid_t buffer[NB_MAX_PROCESSUS]; // Buffer de la FIFO
    int head; // Indice de tête
    int tail; // Indice de queue
    int size; // Taille du buffer
} QUEUE;

bool is_empty(QUEUE *queue);
bool is_full(QUEUE *queue);
void init_queue(QUEUE *queue);
void enqueue(QUEUE *queue, pid_t item);
pid_t dequeue(QUEUE *queue);
void remove_from_queue(QUEUE *q, pid_t pid);

#endif
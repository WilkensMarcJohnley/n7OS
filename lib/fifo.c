/*
    * n7OS - FIFO implementation
    * This file implements a simple FIFO queue for process management.
    * It provides functions to initialize the queue, check if it is empty or full,
    * enqueue and dequeue items, and remove a specific item from the queue.
*/

#include <stdbool.h>
#include <stdio.h>
#include "n7OS/fifo.h"

/*
    * @brief Vérifie si la file d'attente est vide ou pleine
    * 
    * Cette fonction vérifie si la file d'attente est vide ou pleine.
    * Elle est utilisée pour gérer les opérations d'enfilement et de défilement.
    * 
    * @param queue Pointeur vers la file d'attente à vérifier
    * @return true Si la file d'attente est vide
    * @return false Si la file d'attente n'est pas vide
*/
bool is_empty(QUEUE *queue) {
    return queue->size == 0;
}

/*
    * @brief Vérifie si la file d'attente est pleine
    * 
    * Cette fonction vérifie si la file d'attente est pleine.
    * Elle est utilisée pour empêcher l'enfilement d'éléments lorsque la file est pleine.
    * 
    * @param queue Pointeur vers la file d'attente à vérifier
    * @return true Si la file d'attente est pleine
    * @return false Si la file d'attente n'est pas pleine
*/
bool is_full(QUEUE *queue) {
    return queue->size == NB_MAX_PROCESSUS;
}

/*
    * @brief Initialise la file d'attente
    * 
    * Cette fonction initialise les indices de tête et de queue de la file d'attente,
    * ainsi que sa taille. Elle est appelée lors de la création de la file d'attente.
    * 
    * @param queue Pointeur vers la file d'attente à initialiser
*/
void init_queue(QUEUE *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
}


/*
    * @brief Enfile un élément dans la file d'attente
    * 
    * Cette fonction ajoute un élément à la fin de la file d'attente.
    * Elle vérifie d'abord si la file n'est pas pleine avant d'ajouter l'élément.
    * 
    * @param queue Pointeur vers la file d'attente
    * @param item L'élément à ajouter à la file d'attente
*/
void enqueue(QUEUE *queue, pid_t item) {
    if (is_full(queue)) {
        return;
    }
    queue->buffer[queue->tail] = item;
    queue->tail = (queue->tail + 1) % NB_MAX_PROCESSUS;
    queue->size++;
}

/*
    * @brief Défile un élément de la file d'attente
    * 
    * Cette fonction retire et retourne l'élément en tête de la file d'attente.
    * Elle vérifie d'abord si la file n'est pas vide avant de retirer l'élément.
    * 
    * @param queue Pointeur vers la file d'attente
    * @return L'élément retiré de la file d'attente, ou 0 si la file est vide
*/
pid_t dequeue(QUEUE *queue) {
    if (is_empty(queue)) {
        return 0; // Indiquer une erreur ou une valeur spéciale
    }
    pid_t item = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % NB_MAX_PROCESSUS;
    queue->size--;
    return item;
}

/*
    * @brief Retire un processus spécifique de la file d'attente
    * 
    * Cette fonction retire un processus identifié par son PID de la file d'attente.
    * Elle crée une file temporaire pour réinsérer les autres processus.
    * 
    * @param q Pointeur vers la file d'attente
    * @param pid Le PID du processus à retirer
*/
void remove_from_queue(QUEUE *q, pid_t pid) {
    // Créer une file temporaire
    QUEUE temp;
    init_queue(&temp);
    
    // Retirer et réinsérer tous les processus sauf celui à supprimer
    while (!is_empty(q)) {
        pid_t current = dequeue(q);
        if (current != pid) {
            enqueue(&temp, current);
        }
    }
    
    // Remettre les processus dans la file originale
    while (!is_empty(&temp)) {
        enqueue(q, dequeue(&temp));
    }
}


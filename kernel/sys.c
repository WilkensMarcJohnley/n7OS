#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <n7OS/cpu.h>
#include <n7OS/processus.h>

extern void handler_syscall();

int sys_shutdown(int n);

int sys_write(const char *s, int len);

int sys_fork(const char *name, fnptr f);

int sys_exit();

void init_syscall() {
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, sys_example);
  add_syscall(NR_shutdown, sys_shutdown);
  add_syscall(NR_write, sys_write);
  add_syscall(NR_myfork, sys_fork);
  add_syscall(NR_myexit, sys_exit);

  // initialisation de l'IT soft qui gère les appels systeme
  init_irq_entry(0x80, (uint32_t) handler_syscall);
}

// code de la fonction de traitement de l'appel systeme example
int sys_example() {
  // on ne fait que retourner 1
  return 1;
}

//code de la fonction de traitement de l'appel systeme shutdown
/*
  * @brief Implémente l'appel systeme shutdown pour éteindre le système
  * 
  * Cette fonction envoie une commande de mise hors tension au système.
  * Si le paramètre n est égal à 1, elle envoie la commande de mise hors tension.
  * Sinon, elle retourne simplement la valeur de n.
  * 
  * @param n Paramètre pour déterminer si le système doit être éteint ou non
  * @return -1 si le système est éteint, sinon retourne n
*/
int sys_shutdown (int n) {
  if ( n == 1) {
    outw(0x2000 , 0x604 ); // Poweroff qemu > 2.0
    return -1;
  } else
    return n ;
  }

//code de la fonction de traitement de l'appel systeme write
/*
  * @brief Implémente l'appel systeme write pour écrire des données sur la console
  * 
  * Cette fonction écrit une chaîne de caractères sur la console en utilisant la fonction console_putbytes.
  * Elle prend en paramètre un pointeur vers la chaîne de caractères et sa longueur.
  * 
  * @param s Pointeur vers la chaîne de caractères à écrire
  * @param len Longueur de la chaîne de caractères
  * @return La longueur de la chaîne écrite
*/
int sys_write(const char *s, int len) {
  console_putbytes(s, len);
  return len;
}


/*
  * @brief Implémente l'appel fork pour créer un nouveau processus
  * 
  * Cette fonction crée un nouveau processus en utilisant la fonction creer_processus.
  * Elle prend en paramètre le nom du processus et une fonction de type fnptr qui sera exécutée par le nouveau processus.
  * 
  * @param name Le nom du nouveau processus
  * @param f La fonction que le nouveau processus doit exécuter
  * @return Le PID du nouveau processus ou -1 en cas d'échec
*/
int sys_fork(const char *name, fnptr f) {
  // Implémentation de la fonction fork

  pid_t res= creer_processus(name, f);
  if (res == -1) {
    return -1; // Échec de la création du processus
  }else {
    // Si la création du processus a réussi, on retourne le PID du nouveau processus
    return res;
  } 
  
  
}

/*
  * @brief Implémente l'appel systeme exit pour terminer le processus courant
  * 
  * Cette fonction termine le processus courant en appelant la fonction terminer_processus avec le PID du processus courant.
  * Elle appelle ensuite le planificateur pour passer au prochain processus.
  * 
  * @return Toujours 0, indiquant que le processus s'est terminé avec succès
*/
int sys_exit() {
  pid_t pid = getpidCourant(); // Récupérer le PID du processus courant
  terminer_processus(pid); // Terminer le processus courant
  scheduler(); // Appeler le planificateur pour passer au prochain processus
  return 0; // Retourner 0 pour indiquer que le processus s'est terminé avec succès
}

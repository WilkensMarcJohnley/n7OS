#include <stdio.h>
#include <n7OS/cpu.h>
#include <unistd.h>


void processus2() {
  printf("Hello, world from P2\n");
   
  for(int i = 0; i < 1000000; i++) {
    // Simulate some work
    if (i % 200000 == 0) {
      printf("Processus 2 running: %d\n", i);
      //myexit();
    }

     //hlt();// Simulate waiting for the next tick
  }
  printf("Processus 2 finished\n");
  myexit(); // Terminate the process

}
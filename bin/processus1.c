#include <stdio.h>
#include <n7OS/cpu.h>
#include <unistd.h>

void processus1() {
  printf("Hello, world from P1\n");
  int i = 0;

  for(int j = 0; j < 1000000; j++) {
    // Simulate some work
    if (j % 400000 == 0) {
      printf("Processus 1 running: %d\n", j);
    }
  }

  printf("Processus 1 finished\n");
  myexit(); // Terminate the process
}

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cpu_monitor.h"

volatile sig_atomic_t signal_received = 0;

void handle_signal(int sig) {
  if(sig == SIGINT) {
    signal_received = 1;
  }
}

int main(void) {
 pid_t child_id;
  child_id = fork();//lebo fork da predkovy id potomka a potomok dostane 0
  if(child_id < 0) {
    perror("Nepodarilo sa vytvorit fork");
    return 1;
  } else if (child_id == 0) {
    setpgid(0,0); //aby sa otec nezabil skor ako syn
    if(signal(SIGINT,handle_signal) == SIG_ERR){//zevraj ten handle staci takto a jadro by si to malo samo zrobit
      perror("Chyba pri nacitani signalu");
      return EXIT_FAILURE;
    } else {
      run_cpu_monitor();
      return 0;
    }
  } else {
    char c = getchar();
    kill(child_id,SIGINT);//lebo otec by mal mat id syna
    waitpid(child_id,NULL,0);
  }
}
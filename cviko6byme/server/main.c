#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "cpu_monitor.h"
#include "cpu_protocol.h"
#include "socket_utils.h"

#define SERVER_PORT 20001

volatile sig_atomic_t signal_received = 0;

void handle_signal(int sig) {
  if(sig == SIGINT) {
    signal_received = 1;
  }
}
void * watch_for_quit(void * arg) {
  pid_t child_pid = *(pid_t *)arg;
  int nacitany_character;
  while((nacitany_character = getchar()) != EOF) {
    if(nacitany_character == 'q') {
      kill(child_pid,SIGINT);
      break;
    }
  }
  return NULL;
}

int main(void) {
  char buffer[50];
  char send_buffer[50];
  int pipefd[2];
  int pipe_o = pipe(pipefd);
  if(pipe_o == -1) {
    perror("Chyba pri vzniku pipi!");
    return EXIT_FAILURE;
  }
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
      close(pipefd[0]);//citacia lebo syn musi vediet len data zasielat
      run_cpu_monitor(pipefd[1]);//chapes to je ten channel aka filledescriptor
      close(pipefd[1]);
      return 0;
    }
  } else {
    pthread_t thread;
    int errorik;
    if((errorik = pthread_create(&thread,NULL,&watch_for_quit,&child_id))!=0){
      fprintf(stderr,"Chyba pri tvorbe vlakna\n");
      return EXIT_FAILURE;
    }
    close(pipefd[1]);
    //TU
    int passive_socket,active_socket;
    socket_server_init(&passive_socket, SERVER_PORT);
    fprintf(stdout,"Cakanie na pripojenie clienta\n");
    socket_server_accept_connection(&passive_socket, &active_socket);
    fprintf(stdout,"Cleint sa pripojil\n");
    ssize_t velkost;
    while((velkost = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0){//lebo null termination
      buffer[velkost] = '\0';
      printf("%s\n",buffer);//%s ako char[] ako String ktory vlastne je char[]*
      cpu_data_t cpu_data;
      if(deserialize_cpu_data(&cpu_data, buffer)==false) {
        continue;
      }
      size_t send_length;
      serialize_cpu_data(cpu_data,send_buffer, sizeof(send_buffer), &send_length);
      socket_write(&active_socket, send_buffer,send_length);
    }
    close(pipefd[0]);
    pthread_join(thread,NULL);
    socket_server_destroy(&active_socket);
    socket_server_destroy(&passive_socket);
    waitpid(child_id,NULL,0);
  }
}
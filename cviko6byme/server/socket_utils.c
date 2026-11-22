#include "socket_utils.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Vytvorenie TCP socketu, jeho naviazanie na lokálnu adresu a port (bind)
// a prepnutie do pasívneho (počúvajúceho) režimu (listen).
void socket_server_init(int *soc, int port) {
  *soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (*soc < 0) {
    perror("socket_server_init: zlyhanie volania socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  server_address.sin_port = htons(port);

  if (bind(*soc, (const struct sockaddr *)&server_address,
          sizeof(server_address)) < 0) {
    perror("socket_server_init: zlyhanie volania bind");
    exit(EXIT_FAILURE);
  }
  if (listen(*soc, 1) < 0) {
    perror("socket_server_init: zlyhanie volania listen");
    exit(EXIT_FAILURE);
  }
}

void socket_server_destroy(int *soc) {
  close(*soc);
  *soc = -1;
}

// accept() zablokuje volajúce vlákno, kým sa nepripojí klient. Adresu
// klienta si v tomto cvičení nepotrebujeme, preto ju od accept() ani
// nepýtame (NULL, NULL).
void socket_server_accept_connection(int *passive_socket, int *active_socket) {
  *active_socket = accept(*passive_socket, NULL, NULL);
  if (*active_socket < 0) {
    perror("socket_server_accept_connection: zlyhanie volania accept");
    exit(EXIT_FAILURE);
  }
}

ssize_t socket_read(int *soc, char *buffer, size_t length) {
  ssize_t n = read(*soc, buffer, length);
  if (n < 0) {
    perror("socket_read: zlyhanie citania zo socketu");
    exit(EXIT_FAILURE);
  }
  return n;
}

ssize_t socket_write(int *soc, const char *buffer, size_t length) {
  ssize_t n = write(*soc, buffer, length);
  if (n < 0) {
    perror("socket_write: zlyhanie zapisovania do socketu");
    exit(EXIT_FAILURE);
  }
  return n;
}
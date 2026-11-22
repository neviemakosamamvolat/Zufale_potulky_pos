#include "socket_utils.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Zistenie adries servera podľa mena a portu (getaddrinfo) a vytvorenie TCP
// spojenia (socket + connect). getaddrinfo môže vrátiť viac adries naraz
// (IPv4 aj IPv6) - skúšame ich postupne, kým jedna nezafunguje.
void socket_client_init(int *soc, const char *server_name, const char *port) {
  struct addrinfo *server;
  struct addrinfo hints;
  // getaddrinfo vyžaduje, aby boli nepoužité polia hints vynulované - inak
  // ide o nedefinované správanie.
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  int s = getaddrinfo(server_name, port, &hints, &server);
  if (s != 0) {
    fprintf(stderr,
            "socket_client_init: zlyhanie funkcie getaddrinfo(%d): %s\n", s,
            gai_strerror(s));
    exit(EXIT_FAILURE);
  }
  // Pozor! V určitých prípadoch môže existovať aj viac dostupných adries
  for (struct addrinfo *rp = server; rp != NULL; rp = rp->ai_next) {
    // Vytvorenie soketu pre komunikáciu cez internet, pričom sa používa
    // získaná informácia
    *soc = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    // Ak nedošlo k vytvoreniu soketu
    if (*soc < 0) {
      // Pokračuj ďalšou adresou
      continue;
    }
    // Pripojenie na server
    printf("Klient sa pripaja na server!\n");
    if (connect(*soc, rp->ai_addr, rp->ai_addrlen) < 0) {
      // Ak zlyhalo, tak sa zatvorí soket a pokračuje sa ďalšou adresou
      printf("Zlyhanie pripojenia!\n");
      socket_client_destroy(soc);
    } else {
      // Ak sa podarilo pripojiť, uvoľní sa získaná štruktúra z funkcie
      // getaddrinfo a nastaví sa pomocná premenná na úspešné pripojenie
      freeaddrinfo(server);
      return;
    }
  }
  // Pokiaľ po prejdení všetkých adries nedošlo k pripojeniu, tak vypíš chybu a
  // skonči
  fprintf(stderr, "socket_client_init: zlyhalo pripojenie na server!\n");
  freeaddrinfo(server);
  exit(EXIT_FAILURE);
}

void socket_client_destroy(int *soc) {
  close(*soc);
  *soc = -1;
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
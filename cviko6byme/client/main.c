#include <stdio.h>
#include <stdlib.h>
#include "cpu_protocol.h"
#include "socket_utils.h"

int main(int argc, char **argv) {

  if(argc!=3) {
    fprintf(stderr,"%s 127.0.0.1 20001\n",argv[0]);
    return EXIT_FAILURE;
  }else {
    int soc;
    socket_client_init(&soc,argv[1],argv[2]);
    fprintf(stdout,"Klient je pripojeny\n");
    char buffer[100];
    ssize_t maximum;
    while((maximum = socket_read(&soc,buffer, sizeof(buffer)-1))>0){
      buffer[(int)maximum] = '\0';
      cpu_data_t cpu_data;
      if(!deserialize_cpu_data(&cpu_data, buffer)){
        fprintf(stderr, "Chyba pri konverzii dat na cpu_data struct na strane klienta\n");
      }else {
        print_cpu_data(&cpu_data);
      }
    }
    fprintf(stdout,"Prerušenie spojenia so serverom\n");
    socket_client_destroy(&soc);
    return EXIT_SUCCESS;
  }
 // TODO:
 // 1. ak argc nie je rovné 3, vypíšte na stderr návod na použitie
 // (argv[0] <adresa_servera> <port>) a vráťte EXIT_FAILURE,
 // 2. vytvorte premennú int soc a zavolajte socket_client_init(&soc,
 // argv[1], argv[2]); vypíšte hlásenie, že ste pripojení,
 // 3. v cykle čítajte dáta funkciou socket_read do pomocného poľa
// (napr. char buffer[100]), kým vracia kladný počet bajtov;
  // // prečítanú časť NUL-terminujte, spracujte ju funkciou
 // deserialize_cpu_data do cpu_data_t a pri úspechu ju vypíšte
 // funkciou print_cpu_data (pri neúspechu vypíšte chybu na stderr),
 // 4. po skončení cyklu vypíšte hlásenie o ukončení spojenia a zavolajte
 // socket_client_destroy(&soc),
 // 5. vráťte EXIT_SUCCESS.
}
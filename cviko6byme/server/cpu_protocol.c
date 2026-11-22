#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "cpu_protocol.h"

void serialize_cpu_data(const cpu_data_t cpu_data, char *buffer,size_t capacity, size_t *length) {
  memset(buffer, 0,capacity);
  *length =(size_t) snprintf(buffer,capacity,"%s;%.2lf",cpu_data.cpu_name,cpu_data.utilization);
// TODO: vynulujte buffer funkciou memset a funkciou snprintf doň zapíšte
// text v tvare "nazov_cpu;vytazenie" (formátovací reťazec "%s;%.2lf");
// návratovú hodnotu snprintf (počet zapísaných znakov) uložte do *length
}

bool deserialize_cpu_data(cpu_data_t *cpu_data, const char *buffer) {
  int succesion = sscanf(buffer,"%6[^;];%lf",cpu_data->cpu_name,&cpu_data->utilization);
  if(succesion == 2) {
    return true;
  }
// TODO: funkciou sscanf spracujte buffer podľa formátovacieho reťazca
// "%6[^;];%lf" do cpu_data->cpu_name a cpu_data->utilization; vráťte
// true iba vtedy, keď sscanf úspešne načítal obe hodnoty
  return false;
}
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct CPUStats {
  char cpu_name[7];
  unsigned int user;
  unsigned int nice;
  unsigned int system;
  unsigned int idle;
  unsigned int iowait;
  unsigned int irq;
  unsigned int softirq;
} CPUStats;

void init_from_params(CPUStats *self, const char cpu_name[], unsigned int user, unsigned int nice, unsigned int system, unsigned int idle, unsigned int iowait, unsigned int irq, unsigned int softirq) {
  strncpy(self->cpu_name,cpu_name,sizeof(self->cpu_name) - 1);
  self->cpu_name[sizeof(self->cpu_name) - 1] = '\0';//bo vsak index od nuly mongol
  self->user = user;
  self->nice = nice;
  self->system = system;
  self->idle = idle;
  self->iowait = iowait;
  self->irq = irq;
  self->softirq = softirq;
}

_Bool init_from_stat_line(CPUStats *self, const char *line) {
  char cpu_name[7];
  unsigned int user,nice,system,idle,iowait,irq,softirq;
  int n = sscanf(line,"%6s %u %u %u %u %u %u %u",cpu_name,&user,&nice,&system,&idle,&iowait,&irq,&softirq);
  if(n==8) {
    init_from_params(self,cpu_name,user,nice,system,idle,iowait,irq,softirq);
    return true;
  } else {
    return false;
  }
}

void print_cpu_stats(const CPUStats *self) {
  printf("cpu_name:%s , user: %u , nice:%u , system:%u , idle:%u , iowait:%u , irq:%u , softirq:%u ;\n",
      self->cpu_name,self->user,self->nice,self->system,self->idle,self->iowait,self->irq,self->softirq);
}

unsigned int calculate_idle_time(const CPUStats *self) {
  return self->idle + self->iowait;
}

unsigned int calculate_total_time(const CPUStats *self) {
  return self->user + self->nice + self->system + calculate_idle_time(self) + self->irq + self->softirq;
}

double calculate_cpu_utilization(const CPUStats *self, const CPUStats *previous) {
  unsigned int overall = (calculate_total_time(self) - calculate_total_time(previous));
  unsigned int idle = (calculate_idle_time(self) - calculate_idle_time(previous));
  if(overall == 0) {
    return 0.0;
  }
  return (((double)overall  - idle) / overall) * 100;//lebo percenta
}

_Bool read_stat_line(CPUStats *cpu_stats) {
  char *line = NULL;//akoze prazdny inak pri priradil nahodnu adresu
  size_t length = 0;//to iste aj tu len by priradil nahodnu hodnotu
  FILE *file = fopen("/proc/stat","r");
  if(file != NULL) { 
    if(getline(&line, &length, file) != -1 && init_from_stat_line(cpu_stats, line)) {
      free(line);
      fclose(file);
      return true;
    }else {
      fprintf(stderr,"%s\n", "Parsovacia chyba");
      free(line);
      fclose(file);
      return false;
    }
  } else {
    fprintf(stderr,"%s\n", strerror(errno));
    return false;
  
  }
}

int main(int argc, char **argv) {
  CPUStats stat1;
  CPUStats stat2;
  if(read_stat_line(&stat1)) {
    sleep(1);
    if(read_stat_line(&stat2)) {
      print_cpu_stats(&stat1);
      print_cpu_stats(&stat2);
      double util_stat = calculate_cpu_utilization(&stat2, &stat1);
      fprintf(stdout, "Utilization:  %.2lf\n",util_stat);
      return 0;

    }else {
      return EXIT_FAILURE;

    }

  }else {
    return EXIT_FAILURE;//funguj lebo si ubllizim
  }
}
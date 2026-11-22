#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H
#include <stdbool.h>
#include <signal.h>

extern volatile sig_atomic_t signal_received;

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

void init_from_params(CPUStats *self, const char cpu_name[], unsigned int user,unsigned int nice, unsigned int system, unsigned int idle,unsigned int iowait, unsigned int irq,unsigned int softirq);
_Bool init_from_stat_line(CPUStats *self, const char *line);
void print_cpu_stats(const CPUStats *self);
unsigned int calculate_idle_time(const CPUStats *self);
unsigned int calculate_total_time(const CPUStats *self);
double calculate_cpu_utilization(const CPUStats *self,
const CPUStats *previous);
_Bool read_stat_line(CPUStats *cpu_stats);
void run_cpu_monitor(void);

#endif /* ifndef CPU_MONITOR
#define CPU_MONITOR
 */
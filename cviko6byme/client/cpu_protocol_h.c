#ifndef CPU_PROTOCOL_H
#define CPU_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct CPUData {
 char cpu_name[7];
 double utilization;
} cpu_data_t;
void print_cpu_data(const cpu_data_t *cpu_data);
void serialize_cpu_data(const cpu_data_t cpu_data, char *buffer,
size_t capacity, size_t *length);
bool deserialize_cpu_data(cpu_data_t *cpu_data, const char *buffer);
#endif
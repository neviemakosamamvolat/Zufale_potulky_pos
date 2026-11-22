#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stddef.h>
#include <sys/types.h>

void socket_client_init(int *soc, const char *server_name, const char *port);
void socket_client_destroy(int *soc);

ssize_t socket_read(int *soc, char *buffer, size_t length);
ssize_t socket_write(int *soc, const char *buffer, size_t length);

#endif /* ifndef SOCKET_UTILS_H */
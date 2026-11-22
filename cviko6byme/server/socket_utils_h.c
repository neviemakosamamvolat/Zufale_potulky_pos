#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stddef.h>
#include <sys/types.h>

void socket_server_init(int *soc, int port);
void socket_server_destroy(int *soc);
void socket_server_accept_connection(int *passive_socket, int *active_socket);

ssize_t socket_read(int *soc, char *buffer, size_t length);
ssize_t socket_write(int *soc, const char *buffer, size_t length);

#endif /* ifndef SOCKET_UTILS_H */
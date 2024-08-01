#pragma once

#include <string/string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stddef.h>

#define SOCKET_INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKET_TIMEOUT -1

typedef int socket_handle_t;

bool socket_ping(const char* ip_address);
socket_handle_t socket_create_socket(time_t receive_timeout_s, bool tcp);
bool socket_bind_and_listen(socket_handle_t socket, int16_t port);
socket_handle_t socket_accept_incomming_connection(socket_handle_t socket, time_t receive_timeout_us);
bool socket_connect(socket_handle_t socket, const char* ip, unsigned short port);
bool socket_send(socket_handle_t socket, const void* data, size_t size_bytes);
ssize_t socket_receive(socket_handle_t socket, char* data, size_t buffer_size);
void socket_close(socket_handle_t* socket);

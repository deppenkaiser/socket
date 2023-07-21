#pragma once

#include <stdbool.h>
#include <sys/time.h>

#define SOCKET_INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int socket_handle_t;

socket_handle_t socket_create_socket(time_t receive_timeout_s);
bool socket_bind_and_listen(socket_handle_t socket);
socket_handle_t socket_accept_incomming_connection(socket_handle_t socket, time_t receive_timeout_us);
void socket_close(socket_handle_t* psocket);

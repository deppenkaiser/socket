#pragma once

#include <stdbool.h>

#define SOCKET_INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int socket_handle_t;

socket_handle_t socket_create_socket();
bool socket_bind_and_listen(socket_handle_t socket);
socket_handle_t socket_accept_incomming_connection(socket_handle_t socket);
void socket_close(socket_handle_t* psocket);

#pragma once

#ifdef LINUX
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <stdbool.h>

    typedef int socket_handle_t;

    #define SOCKET_INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif // LINUX

#ifdef WINDOWS
    #include <WinSock2.h>    
    typedef SOCKET socket_handle_t;
#endif // WINDOWS

socket_handle_t socket_create_socket();
bool socket_bind_and_listen(socket_handle_t socket);
socket_handle_t socket_accept_incomming_connection(socket_handle_t socket);
void socket_close(socket_handle_t* psocket);

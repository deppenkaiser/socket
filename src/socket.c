#include "socket/socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

socket_handle_t socket_create_socket(time_t receive_timeout_s)
{
    socket_handle_t receive = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv_connect = {0};

    tv_connect.tv_sec = receive_timeout_s;
    tv_connect.tv_usec = 0;

    if (receive != SOCKET_INVALID_SOCKET)
    {
        if (setsockopt(receive, SOL_SOCKET, SO_RCVTIMEO, &tv_connect, sizeof(tv_connect)) == SOCKET_ERROR)
        {
            socket_close(&receive);
        }
    }

    return receive;
}

bool socket_bind_and_listen(socket_handle_t socket)
{
    bool bRetVal = false;
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    if (bind(socket, (struct sockaddr*) &server, sizeof(server)) != SOCKET_ERROR)
    {
        if (listen(socket, 1) != SOCKET_ERROR)
        {
            bRetVal = true;
        }
    }

    return bRetVal;
}

socket_handle_t socket_accept_incomming_connection(socket_handle_t socket, time_t receive_timeout_us)
{
    bool bRetVal = false;
    struct sockaddr_in clientAddr = {0};
    socklen_t nSizeBytes = sizeof(clientAddr);
    socket_handle_t client = accept(socket, (struct sockaddr*) &clientAddr, &nSizeBytes);
    struct timeval tv = {0};

    tv.tv_sec = 0;
    tv.tv_usec = receive_timeout_us;

    if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == SOCKET_ERROR)
    {
        socket_close(&client);
    }

    return client;
}

void socket_close(socket_handle_t* psocket)
{
    close(*psocket);
    *psocket = SOCKET_INVALID_SOCKET;
}

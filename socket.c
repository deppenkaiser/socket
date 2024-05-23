#include "socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

socket_handle_t socket_create_socket(time_t receive_timeout_s, bool tcp)
{
    socket_handle_t receive = socket(AF_INET, tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
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
    bool bind_and_listen = false;
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    if (bind(socket, (struct sockaddr*) &server, sizeof(server)) != SOCKET_ERROR)
    {
        if (listen(socket, 1) != SOCKET_ERROR)
        {
            bind_and_listen = true;
        }
    }

    return bind_and_listen;
}

socket_handle_t socket_accept_incomming_connection(socket_handle_t socket, time_t receive_timeout_us)
{
    struct sockaddr_in client_addr = {0};
    socklen_t size_bytes = sizeof(client_addr);
    socket_handle_t client = accept(socket, (struct sockaddr*) &client_addr, &size_bytes);
    struct timeval tv = {0};

    tv.tv_sec = 0;
    tv.tv_usec = receive_timeout_us;

    if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == SOCKET_ERROR)
    {
        socket_close(&client);
    }

    return client;
}

bool socket_connect(socket_handle_t socket, const char* ip, unsigned short port)
{
    bool connected = false;
    struct sockaddr_in server = {0};
    struct timeval tv = {0};

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

    if (setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != SOCKET_ERROR)
    {
        if (connect(socket, (struct sockaddr*) &server , sizeof(server)) != SOCKET_ERROR)
        {
            // prevent exception, if the mount server closes the connection
            signal(SIGPIPE, SIG_IGN);
            connected = true;
        }
    }
    else
    {
        socket_close(&socket);
    }

    return connected;
}

bool socket_send(socket_handle_t socket, const void* data, size_t size_bytes)
{
    bool sent = false;

    ssize_t bytes_send = send(socket, data, size_bytes, MSG_NOSIGNAL);

    if (bytes_send == size_bytes)
    {
        sent = true;
    }

    return sent;
}

size_t socket_receive(socket_handle_t socket, char* data, size_t buffer_size)
{
    return recv(socket, data, buffer_size, 0);
}

void socket_close(socket_handle_t* socket)
{
    if (*socket != SOCKET_INVALID_SOCKET)
    {
        close(*socket);
        *socket = SOCKET_INVALID_SOCKET;
    }
}

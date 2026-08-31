#include "socket/socket.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

bool socket_ping(const char* ip_address)
{
    char command[100] = {0};
    snprintf(command, sizeof(command), "ping -c 1 %s", ip_address);
    return system(command) == 0;
}

socket_handle_t socket_create(time_t receive_timeout_s, bool tcp)
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

bool socket_bind_and_listen(socket_handle_t socket, int16_t port)
{
    bool bind_and_listen = false;
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

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
    return accept(socket, (struct sockaddr*) &client_addr, &size_bytes);
}

bool socket_connect(socket_handle_t socket, const char* ip, unsigned short port)
{
    bool connected = false;
    struct sockaddr_in server = {0};

    server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

    if (connect(socket, (struct sockaddr*) &server , sizeof(server)) != SOCKET_ERROR)
    {
        // prevent exception, if the mount server closes the connection
        signal(SIGPIPE, SIG_IGN);
        connected = true;
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

ssize_t socket_receive(socket_handle_t socket, char* data, size_t buffer_size)
{
    return recv(socket, data, buffer_size, 0);
}

bool socket_udp_broadcast(const char* broadcast_ip, unsigned short port, const void* data, size_t size_bytes, char* sender_ip, size_t sender_ip_size)
{
    bool received = false;
    socket_handle_t udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket != SOCKET_INVALID_SOCKET)
    {
        int broadcast_enable = 1;
        struct sockaddr_in broadcast_addr = {0};

        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_addr.s_addr = inet_addr(broadcast_ip);
        broadcast_addr.sin_port = htons(port);

        if (setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) != SOCKET_ERROR &&
                sendto(udp_socket, data, size_bytes, 0, (struct sockaddr*) &broadcast_addr, sizeof(broadcast_addr)) != SOCKET_ERROR)
        {
            fd_set read_fds;
            struct timeval timeout = {0};

            FD_ZERO(&read_fds);
            FD_SET(udp_socket, &read_fds);
            timeout.tv_sec = 3;

            if (select(udp_socket + 1, &read_fds, NULL, NULL, &timeout) > 0)
            {
                struct sockaddr_in sender_addr = {0};
                socklen_t sender_size = sizeof(sender_addr);

                if (recvfrom(udp_socket, NULL, 0, 0, (struct sockaddr*) &sender_addr, &sender_size) != SOCKET_ERROR)
                {
                    const char* ip = inet_ntoa(sender_addr.sin_addr);
                    if (ip != NULL && sender_ip_size > 0)
                    {
                        snprintf(sender_ip, sender_ip_size, "%s", ip);
                        received = true;
                    }
                }
            }
        }

        close(udp_socket);
    }

    return received;
}

void socket_close(socket_handle_t* socket)
{
    if (*socket != SOCKET_INVALID_SOCKET)
    {
        close(*socket);
        *socket = SOCKET_INVALID_SOCKET;
    }
}

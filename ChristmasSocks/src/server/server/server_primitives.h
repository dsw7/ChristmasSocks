#ifndef SERVER_PRIMITIVES_H
#define SERVER_PRIMITIVES_H

#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr
#include "logger.h"
#include "constants.h"

class ServerPrimitives
{
    private:
        unsigned int tcp_port;
        unsigned int backlog;
        std::string  bind_ip;

    public:
        int          socket_fd_server; // must be public - used in epoll class
        struct       sockaddr_in address;

        ServerPrimitives(
            unsigned int &tcp_port,
            unsigned int &backlog,
            std::string bind_ip
        );

        bool open_server_socket_file_descriptor();
        bool close_server_socket_file_descriptor();
        bool attach_socket_file_descriptor_to_port();
        bool bind_socket_file_descriptor_to_port();
        bool listen_on_bound_tcp_port();
};

#endif
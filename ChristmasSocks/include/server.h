#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "logger.h"

class Server {
    private:
        int tcp_port;
        int max_connections_queue;

    public:
        Server(int max_connections_queue);

        bool open_server_socket_file_descriptor();
        bool close_server_socket_file_descriptor();
        bool attach_socket_file_descriptor_to_port();
        bool bind_socket_file_descriptor_to_port();
        bool listen_on_bound_tcp_port();

        int socket_fd_server;
        struct sockaddr_in address;
};

#endif

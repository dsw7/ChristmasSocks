#ifndef CLIENTS_H
#define CLIENTS_H

#include "logger.h"
#include <arpa/inet.h> // inet_ntoa
#include <string.h>
#include <errno.h>

namespace ClientHandler {
    bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client);
    bool close_client_socket_file_descriptor(int &socket_fd_client);
    bool read_data(std::string &message, int &socket_fd_client);
    bool write_data(std::string &message, int &socket_fd_client);
}

#endif

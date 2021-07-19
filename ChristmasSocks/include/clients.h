#ifndef CLIENTS_H
#define CLIENTS_H

#include <arpa/inet.h> // inet_ntoa
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "logger.h"
#include "constants.h"
#include "sysconfig_pipeline.h"

class Client
{
    private:
        int buffer_size;

    public:
        Client(configs_t &configs);

        bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client);
        bool close_client_socket_file_descriptor(int &socket_fd_client);
        bool read_data(std::string &message, int &socket_fd_client);
        bool write_data(std::string &message, int &socket_fd_client);
};

#endif

#ifndef INCOMING_CLIENT_PRIMITIVES_H
#define INCOMING_CLIENT_PRIMITIVES_H

#include <arpa/inet.h> // inet_ntoa
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "logger.h"
#include "constants.h"
#include "get_line_breaks.h"

class IncomingClientPrimitives
{
    private:
        int         tcp_buffer_size;
        std::string newline;

    public:
        IncomingClientPrimitives(
            int &tcp_buffer_size
        );

        bool is_valid_buffer_size();
        bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client);
        bool close_client_socket_file_descriptor(int &socket_fd_client);
        bool read_data(std::string &message, int &socket_fd_client);
        bool write_data(std::string &message, int &socket_fd_client);
};

#endif

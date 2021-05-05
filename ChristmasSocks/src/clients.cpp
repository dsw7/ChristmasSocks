#include "clients.h"

namespace ClientHandler {

    /* https://linux.die.net/man/3/accept */
    bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client) {
        int addrlen = sizeof(address);

        socket_fd_client = accept(
            socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&addrlen
        );

        if (socket_fd_client == -1) {
            ClientLogger::error(strerror(errno));
            return false;
        }

        char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
        ClientLogger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
        ClientLogger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

        return true;
    }

    /* https://linux.die.net/man/3/close */
    bool close_client_socket_file_descriptor(int &socket_fd_client) {
        ClientLogger::info("Closing client socket file descriptor: " + std::to_string(socket_fd_client));

        if (close(socket_fd_client) == -1) {
            ClientLogger::error(strerror(errno));
            return false;
        }
        return true;
    }

    /* https://linux.die.net/man/3/read */
    bool read_data(std::string &message, int &socket_fd_client) {
        char buffer[TCP_BUFFER_SIZE] = {0};

        int rv = read(socket_fd_client, buffer, TCP_BUFFER_SIZE);
        message = std::string(buffer);

        if (rv < 0) {
            ClientLogger::error(strerror(errno));
            return false;
        } else if (rv == 0) {
            ClientLogger::info("Socket read() received EOF - client hang up detected");
            close_client_socket_file_descriptor(socket_fd_client);
            return false;
        } else {
            ClientLogger::info("Read in message '" + message + "' from descriptor " + std::to_string(socket_fd_client));
        }

        return true;
    }

    /* https://linux.die.net/man/3/send */
    bool write_data(std::string &message, int &socket_fd_client) {
        int rv = send(socket_fd_client, message.c_str(), message.size(), 0);

        if (rv == -1) {
            ClientLogger::error(strerror(errno));
            return false;
        }

        ClientLogger::info("Wrote out message '" + message + "' to descriptor " + std::to_string(socket_fd_client));
        return true;
    }

}

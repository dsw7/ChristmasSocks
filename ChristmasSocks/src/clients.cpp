#include "clients.h"

Client::Client(configs_t &configs) {
    this->buffer_size = configs.tcp_buffer_size;
}

/* https://linux.die.net/man/3/accept */
bool Client::accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client) {
    int addrlen = sizeof(address);

    socket_fd_client = accept(
        socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&addrlen
    );

    if (socket_fd_client == -1) {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }

    char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
    ClientLogger::info("The kernel has allocated a new client socket file descriptor", socket_fd_client);
    ClientLogger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address), socket_fd_client);
    return true;
}

/* https://linux.die.net/man/3/close */
bool Client::close_client_socket_file_descriptor(int &socket_fd_client) {
    ClientLogger::info("Closing client socket file descriptor", socket_fd_client);

    if (close(socket_fd_client) == -1) {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }
    return true;
}

/* https://linux.die.net/man/3/read */
bool Client::read_data(std::string &message, int &socket_fd_client) {
    char buffer[this->buffer_size] = {0};

    int rv = read(socket_fd_client, buffer, this->buffer_size);
    message = std::string(buffer);

    if (rv < 0) {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    } else if (rv == 0) {
        ClientLogger::info("Socket read() received EOF - client hang up detected", socket_fd_client);
        close_client_socket_file_descriptor(socket_fd_client);
        return false;
    } else {
        ClientLogger::info("Read in message '" + message + "'", socket_fd_client);
    }

    return true;
}

/* https://linux.die.net/man/3/send */
bool Client::write_data(std::string &message, int &socket_fd_client) {
    int rv = send(socket_fd_client, message.c_str(), message.size(), 0);

    if (rv == -1) {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }

    ClientLogger::info("Wrote out message '" + message + "'", socket_fd_client);
    return true;
}

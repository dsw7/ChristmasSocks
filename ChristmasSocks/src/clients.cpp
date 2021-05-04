#include "clients.h"

bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client) {
    int addrlen = sizeof(address);

    // https://linux.die.net/man/3/accept
    socket_fd_client = accept(
        socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&addrlen
    );

    if (socket_fd_client == -1) {
        Logger::error(strerror(errno));
        return false;
    }

    char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
    Logger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
    Logger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

    return true;
}

bool close_client_socket_file_descriptor(int &socket_fd_client) {
    Logger::info("Closing client socket file descriptor: " + std::to_string(socket_fd_client));

    // https://linux.die.net/man/3/close
    if (close(socket_fd_client) == -1) {
        Logger::error(strerror(errno));
        return false;
    }
    return true;
}

bool read_data(std::string &message, int &socket_fd_client) {
    char buffer[TCP_BUFFER_SIZE] = {0};

    // https://linux.die.net/man/3/read
    int rv = read(socket_fd_client, buffer, TCP_BUFFER_SIZE);
    message = std::string(buffer);

    if (rv < 0) {
        Logger::error(strerror(errno));
        return false;
    } else if (rv == 0) {
        Logger::info("Socket read() received EOF - client hang up detected");
        close_client_socket_file_descriptor(socket_fd_client);
        return false;
    } else {
        Logger::info("Read in message '" + message + "' from descriptor " + std::to_string(socket_fd_client));
    }

    return true;
}

bool write_data(std::string &message, int &socket_fd_client) {
    // https://linux.die.net/man/3/send
    int rv = send(socket_fd_client, message.c_str(), message.size(), 0);

    if (rv == -1) {
        Logger::error(strerror(errno));
        return false;
    }

    Logger::info("Wrote out message '" + message + "' to descriptor " + std::to_string(socket_fd_client));
    return true;
}

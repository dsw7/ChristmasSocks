#include "server.h"

Server::Server(int tcp_port, int max_connections_queue) {
    this->tcp_port = tcp_port;
    this->max_connections_queue = max_connections_queue;
}

bool Server::open_server_socket_file_descriptor() {
    // https://linux.die.net/man/3/socket
    this->socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd_server > -1) {
        Logger::info("The kernel has allocated a new server socket file descriptor " + std::to_string(this->socket_fd_server));
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to open socket file descriptor");

    return false;
}

bool Server::close_server_socket_file_descriptor() {
    Logger::info("Closing server socket file descriptor " + std::to_string(this->socket_fd_server));

    // https://linux.die.net/man/3/close
    if (close(this->socket_fd_server) == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to close socket file descriptor");

    return false;
}

bool Server::attach_socket_file_descriptor_to_port() {
    Logger::info("Attaching socket file descriptor " + std::to_string(this->socket_fd_server) + " to TCP port");

    int optval = 1;

    int rv = setsockopt(
        this->socket_fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)
    );

    if (rv == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to attach socket file descriptor to TCP port");

    return false;
}

bool Server::bind_socket_file_descriptor_to_port() {

    if (this->tcp_port <= 1024) {
        Logger::error("All TCP ports below 1024 are reserved!");
        return false;
    } else if (this->tcp_port > 65535) {
        Logger::error("Cannot bind to a TCP port exceeding 65535!");
        return false;
    } else {
        Logger::info("Binding socket file descriptor " + std::to_string(this->socket_fd_server) + " to TCP port");
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->tcp_port);

    // https://linux.die.net/man/3/bind
    int rv = bind(this->socket_fd_server, (struct sockaddr *)&this->address, sizeof(this->address));

    if (rv == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to bind socket file descriptor to TCP port");

    return false;
}

bool Server::listen_on_bound_tcp_port() {
    // https://linux.die.net/man/3/listen
    int rv = listen(this->socket_fd_server, this->max_connections_queue);

    if (rv == 0) {
        Logger::info("Listening on TCP port " + std::to_string(this->tcp_port));
        Logger::info("Will queue a maximum of " + std::to_string(this->max_connections_queue) + " connections");
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Cannot listen on TCP port " + std::to_string(this->tcp_port));

    return false;
}

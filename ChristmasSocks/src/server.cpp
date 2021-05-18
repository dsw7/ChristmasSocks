#include "server.h"

Server::Server(configs_t &configs)
{
    this->tcp_port = configs.tcp_port;
    this->max_connections_queue = configs.max_num_connections_queue;
    this->socket_fd_server = -1;
}

/* https://linux.die.net/man/3/socket */
bool Server::open_server_socket_file_descriptor()
{
    this->socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd_server > -1)
    {
        ServerLogger::info("The kernel has allocated a new server socket file descriptor", this->socket_fd_server);
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Failed to open socket file descriptor", this->socket_fd_server);

    return false;
}

/* https://linux.die.net/man/3/close */
bool Server::close_server_socket_file_descriptor()
{
    ServerLogger::info("Closing server socket file descriptor", this->socket_fd_server);

    if (close(this->socket_fd_server) == 0)
    {
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Failed to close socket file descriptor", this->socket_fd_server);

    return false;
}

/* https://linux.die.net/man/3/setsockopt */
bool Server::attach_socket_file_descriptor_to_port()
{
    ServerLogger::info("Attaching socket file descriptor to TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    int optval = 1;

    int rv = setsockopt(
        this->socket_fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)
    );

    if (rv == 0)
    {
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Failed to attach socket file descriptor to TCP port", this->socket_fd_server);

    return false;
}

/* https://linux.die.net/man/3/bind */
bool Server::bind_socket_file_descriptor_to_port()
{
    if (this->tcp_port <= 1024)
    {
        ServerLogger::error("All TCP ports below 1024 are reserved!", this->socket_fd_server);
        return false;
    }
    else if (this->tcp_port > 65535)
    {
        ServerLogger::error("Cannot bind to a TCP port exceeding 65535!", this->socket_fd_server);
        return false;
    }
    else
    {
        ServerLogger::info("Binding socket file descriptor to TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->tcp_port);

    int rv = bind(this->socket_fd_server, (struct sockaddr *)&this->address, sizeof(this->address));

    if (rv == 0)
    {
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Failed to bind socket file descriptor to TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    return false;
}

/* https://linux.die.net/man/3/listen */
bool Server::listen_on_bound_tcp_port()
{
    int rv = listen(this->socket_fd_server, this->max_connections_queue);

    if (rv == 0)
    {
        ServerLogger::info("Listening on TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);
        ServerLogger::info("Will queue a maximum of " + std::to_string(this->max_connections_queue) + " connections", this->socket_fd_server);
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Cannot listen on TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    return false;
}

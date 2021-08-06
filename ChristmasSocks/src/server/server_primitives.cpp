#include "server_primitives.h"

ServerPrimitives::ServerPrimitives(unsigned int &tcp_port, unsigned int &max_num_connections_queue, std::string bind_ip)
{
    this->tcp_port = tcp_port;
    this->max_num_connections_queue = max_num_connections_queue;
    this->bind_ip = bind_ip;
    this->socket_fd_server = -1;
}

/* https://linux.die.net/man/3/socket */
bool ServerPrimitives::open_server_socket_file_descriptor()
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
bool ServerPrimitives::close_server_socket_file_descriptor()
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
bool ServerPrimitives::attach_socket_file_descriptor_to_port()
{
    ServerLogger::info("Attaching socket file descriptor to TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    int optval = 1;

    int rv = setsockopt(
        this->socket_fd_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)
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
bool ServerPrimitives::bind_socket_file_descriptor_to_port()
{
    if (this->tcp_port <= MINIMUM_TCP_PORT)
    {
        ServerLogger::error("All TCP ports below " + std::to_string(MINIMUM_TCP_PORT) + " are reserved!", this->socket_fd_server);
        return false;
    }
    else if (this->tcp_port > MAXIMUM_TCP_PORT)
    {
        ServerLogger::error("Cannot bind to a TCP port exceeding " + std::to_string(MAXIMUM_TCP_PORT), this->socket_fd_server);
        return false;
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = inet_addr(this->bind_ip.c_str());
    this->address.sin_port = htons(this->tcp_port);

    ServerLogger::info("Binding socket file descriptor to TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    if (this->bind_ip.compare(BIND_IP_INADDR_ANY) == 0)
    {
        ServerLogger::warning("Binding socket file descriptor to IPv4 address " + this->bind_ip, this->socket_fd_server);
        ServerLogger::warning("The listening socket will bind to all available interfaces!", this->socket_fd_server);
    }
    else
    {
        ServerLogger::info("Binding socket file descriptor to IPv4 address " + this->bind_ip, this->socket_fd_server);
    }

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
bool ServerPrimitives::listen_on_bound_tcp_port()
{
    int rv = listen(this->socket_fd_server, this->max_num_connections_queue);

    if (rv == 0)
    {
        ServerLogger::info("Listening on TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);
        ServerLogger::info("Will queue a maximum of " + std::to_string(this->max_num_connections_queue) + " connections", this->socket_fd_server);
        return true;
    }

    ServerLogger::error(strerror(errno), this->socket_fd_server);
    ServerLogger::error("Cannot listen on TCP port " + std::to_string(this->tcp_port), this->socket_fd_server);

    return false;
}

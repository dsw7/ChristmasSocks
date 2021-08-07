#include "incoming_client_primitives.h"

std::string get_line_break(std::string &message)
{
    if (message.compare(-2, 2, "\r\n") == 0)
    {
        return "\r\n";
    }

    std::string line_break;
    if (message.compare(-1, 1, "\n") == 0)
    {
        line_break = "\n";
    }
    else if (message.compare(-1, 1, "\r") == 0)
    {
        line_break = "\r";
    }
    else
    {
        line_break = "";
    }
    return line_break;
}

IncomingClientPrimitives::IncomingClientPrimitives(unsigned int &tcp_buffer_size, bool &handle_line_breaks)
{
    this->buffer_size = tcp_buffer_size;
    this->handle_line_breaks = handle_line_breaks;
}

bool IncomingClientPrimitives::is_valid_buffer_size()
{
    if (this->buffer_size < MINIMUM_TCP_BUFFER_SIZE)
    {
        RootLogger::error("Invalid buffer size. Minimum buffer size is " + std::to_string(MINIMUM_TCP_BUFFER_SIZE) + " bytes");
        return false;
    }
    return true;
}

/* https://linux.die.net/man/3/accept */
bool IncomingClientPrimitives::accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client)
{
    int addrlen = sizeof(address);

    socket_fd_client = accept(
        socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&addrlen
    );

    if (socket_fd_client == -1)
    {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }

    char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
    ClientLogger::info("The kernel has allocated a new client socket file descriptor", socket_fd_client);
    ClientLogger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address), socket_fd_client);
    return true;
}

/* https://linux.die.net/man/3/close */
bool IncomingClientPrimitives::close_client_socket_file_descriptor(int &socket_fd_client)
{
    ClientLogger::info("Closing client socket file descriptor", socket_fd_client);

    if (close(socket_fd_client) == -1)
    {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }
    return true;
}

/* https://linux.die.net/man/3/read */
bool IncomingClientPrimitives::read_data(std::string &message, int &socket_fd_client)
{
    char buffer[this->buffer_size] = {0};

    int rv = read(socket_fd_client, buffer, this->buffer_size);
    message = std::string(buffer);

    if (this->handle_line_breaks)
    {
        this->newline = get_line_break(message);
        message = message.substr(0, message.size() - 1);
    }

    if (rv < 0)
    {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }
    else if (rv == 0)
    {
        ClientLogger::info("Socket read() received EOF - client hang up detected", socket_fd_client);
        close_client_socket_file_descriptor(socket_fd_client);
        return false;
    }
    else
    {
        ClientLogger::info("Read in message '" + message + "'", socket_fd_client);
    }

    return true;
}

/* https://linux.die.net/man/3/send */
bool IncomingClientPrimitives::write_data(std::string &message, int &socket_fd_client)
{
    if (this->handle_line_breaks)
    {
        message = message + this->newline;
    }

    int rv = send(socket_fd_client, message.c_str(), message.size(), 0);

    if (rv == -1)
    {
        ClientLogger::error(strerror(errno), socket_fd_client);
        return false;
    }

    ClientLogger::info("Wrote out message '" + message + "'", socket_fd_client);
    return true;
}

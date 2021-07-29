#include "epoll_server.h"

void ServerImplMain::server_setup()
{
    EventLogger::info("Setting up server primitives", this->epoll_fd);

    if (!open_server_socket_file_descriptor())
    {
        exit(EXIT_FAILURE);
    }

    if (!attach_socket_file_descriptor_to_port())
    {
        exit(EXIT_FAILURE);
    }

    if (!bind_socket_file_descriptor_to_port())
    {
        exit(EXIT_FAILURE);
    }

    if (!listen_on_bound_tcp_port())
    {
        exit(EXIT_FAILURE);
    }
}

/* https://linux.die.net/man/2/epoll_create1 */
void ServerImplMain::open_epoll_file_descriptor()
{
    EventLogger::info("Creating epoll file descriptor", this->epoll_fd);

    this->epoll_fd = epoll_create1(0);

    if (this->epoll_fd == -1)
    {
        EventLogger::error(strerror(errno), this->epoll_fd);
        exit(EXIT_FAILURE);
    }
}

/* https://linux.die.net/man/2/epoll_ctl */
void ServerImplMain::register_server_fd_to_epoll_instance()
{
    EventLogger::info("Registering server file descriptor to epoll instance", this->epoll_fd);

    this->ev.events = EPOLLIN;
    this->ev.data.fd = socket_fd_server;

    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket_fd_server, &this->ev) == -1)
    {
        EventLogger::error(strerror(errno), this->epoll_fd);
        exit(EXIT_FAILURE);
    }
}

void ServerImplMain::loop()
{
    int nfds, socket_fd_client_to_struct, socket_fd_client_from_struct;
    struct epoll_event events[MAX_EPOLL_EVENTS];

    std::string message;
    EventLogger::info("Starting epoll event loop", this->epoll_fd);

    while (true)
    {

        nfds = epoll_wait(this->epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1)
        {
            EventLogger::error(strerror(errno), this->epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == socket_fd_server)
            {

                accept_incoming_connection(
                    socket_fd_server, address, socket_fd_client_to_struct
                );

                //setnonblocking(socket_fd_client_to_struct);
                this->ev.events = EPOLLIN | EPOLLET;
                this->ev.data.fd = socket_fd_client_to_struct;

                if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, socket_fd_client_to_struct, &this->ev) == -1)
                {
                    EventLogger::error(strerror(errno), this->epoll_fd);
                    exit(EXIT_FAILURE);
                }

            }
            else
            {
                message.clear();
                socket_fd_client_from_struct = events[n].data.fd;

                if (read_data(message, socket_fd_client_from_struct))
                {
                    if (message.compare("exit") == 0)
                    {
                        goto endloop;
                    }
                    else
                    {
                        write_data(message, socket_fd_client_from_struct);
                    }
                }
            }
        }
    }

    endloop:;
    EventLogger::info("Ending epoll event loop", this->epoll_fd);
}

void ServerImplMain::server_teardown()
{
    close_server_socket_file_descriptor();
}

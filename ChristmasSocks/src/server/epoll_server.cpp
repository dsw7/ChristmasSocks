#include "epoll_server.h"

void server_impl_main(configs_t &configs)
{
    ServerPrimitives server(configs);
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    IncomingClientPrimitives client(configs);

    struct epoll_event ev, events[MAX_EPOLL_EVENTS];
    int nfds;

    int epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        RootLogger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server.socket_fd_server;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server.socket_fd_server, &ev) == -1)
    {
        RootLogger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    int socket_fd_client_to_struct, socket_fd_client_from_struct;
    std::string message;

    while (true)
    {

        nfds = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1)
        {
            RootLogger::error(strerror(errno));
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == server.socket_fd_server)
            {

                client.accept_incoming_connection(
                    server.socket_fd_server, server.address, socket_fd_client_to_struct
                );

                //setnonblocking(socket_fd_client_to_struct);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = socket_fd_client_to_struct;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_fd_client_to_struct, &ev) == -1)
                {
                    RootLogger::error(strerror(errno));
                    exit(EXIT_FAILURE);
                }

            }
            else
            {
                message.clear();
                socket_fd_client_from_struct = events[n].data.fd;

                if (client.read_data(message, socket_fd_client_from_struct))
                {
                    if (message.compare("exit") == 0)
                    {
                        goto endloop;
                    }
                    else
                    {
                        client.write_data(message, socket_fd_client_from_struct);
                    }
                }
            }
        }
    }

    endloop:;
    server.close_server_socket_file_descriptor();
}

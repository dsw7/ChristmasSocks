#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "help_menu.h"
#include "logger.h"
#include "server_primitives.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include "sysconfig_pipeline.h"
#include "header_footer.h"

void server_impl_main(configs_t &configs)
{
    ServerPrimitives server(configs);
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    Client client(configs);

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

int main(int argc, char **argv)
{
    if (help_message_lazy_runner(argc, argv))
    {
        std::cout << "Exiting..." << std::endl;
        exit(EXIT_SUCCESS);
    }

    display_header();
    register_ipc_signals();

    configs_t global_configs;
    set_root_configs(global_configs);
    overwrite_root_configs_with_config_file_configs(global_configs, CONFIG_FILEPATH);
    overwrite_config_file_configs_with_cli_args(global_configs, argc, argv);

    server_impl_main(global_configs);
    RootLogger::info("Program appears to have exited normally");
    display_footer();
    return 0;
}

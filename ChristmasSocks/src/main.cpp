#include "main.h"

void server_impl_main(configs_t &configs)
{
    Server server(configs);
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
                    client.write_data(message, socket_fd_client_from_struct);
                }
            }
        }
    }

    server.close_server_socket_file_descriptor();
}

int main(int argc, char **argv)
{

    if (help_message_lazy_runner(argc, argv))
    {
        std::cout << "Exiting..." << std::endl;
        exit(EXIT_SUCCESS);
    }

    int c;

    int tcp_port = 8080;
    int tcp_buffer_size = 1024;

    while (1)
    {
        static struct option long_options[] =
        {
            {"port",        required_argument, 0, 'p'},
            {"buffer_size", required_argument, 0, 'b'}
        };

        // What's the point of this?
        int option_index = 0;

        c = getopt_long(
            argc, argv, "p:b:", long_options, &option_index
        );

        // End of options
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'p':
                tcp_port = atoi(optarg);
                break;
            case 'b':
                tcp_buffer_size = atoi(optarg);
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };

    RootLogger::header();
    register_ipc_signals();

    configs_t global_configs;
    set_root_configs(global_configs);
    overwrite_default_configs_with_config_file_configs(global_configs, CONFIG_FILEPATH);

    // find some clever way to override global_configs with stuff from getopt

    server_impl_main(global_configs);

    RootLogger::footer();
    return 0;
}

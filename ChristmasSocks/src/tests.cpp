#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include "config_file.h"
#include <sys/epoll.h>

int main() {
    RootLogger::header();

    read_config_file();
    register_ipc_signals();

    struct epoll_event ev, events[MAX_EPOLL_EVENTS];
    int nfds;

    Server server;  // need to pass a configs_t struct of system configs to this constructor
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    Client client;  // need to pass configs_t struct of configs to this constructor

    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        RootLogger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server.socket_fd_server;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server.socket_fd_server, &ev) == -1) {
        RootLogger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    int socket_fd_client_to_struct, socket_fd_client_from_struct;
    std::string message;

    while (true) {

        nfds = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1) {
            RootLogger::error(strerror(errno));
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server.socket_fd_server) {

                client.accept_incoming_connection(
                    server.socket_fd_server, server.address, socket_fd_client_to_struct
                );

                //setnonblocking(socket_fd_client_to_struct);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = socket_fd_client_to_struct;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_fd_client_to_struct, &ev) == -1) {
                    RootLogger::error(strerror(errno));
                    exit(EXIT_FAILURE);
                }

            } else {
                message.clear();
                socket_fd_client_from_struct = events[n].data.fd;

                if (client.read_data(message, socket_fd_client_from_struct)) {
                    client.write_data(message, socket_fd_client_from_struct);
                }
            }
        }
    }

    server.close_server_socket_file_descriptor();
    RootLogger::footer();
    return 0;
}

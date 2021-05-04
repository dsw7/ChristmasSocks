#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main() {
    Logger::header();
    register_ipc_signals();

    struct epoll_event ev, events[MAX_EVENTS];
    int socket_fd_client, nfds, epollfd;

    Server server(TCP_PORT, MAX_NUM_CONNECTIONS_QUEUE);
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    epollfd = epoll_create1(0);

    if (epollfd == -1) {
        Logger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server.socket_fd_server;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server.socket_fd_server, &ev) == -1) {
        Logger::error(strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (true) {

        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            Logger::error(strerror(errno));
            exit(EXIT_FAILURE);
        }

        Logger::info("An event has been detected");

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server.socket_fd_server) {
                accept_incoming_connection(server.socket_fd_server, server.address, socket_fd_client);

                //setnonblocking(socket_fd_client);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = socket_fd_client;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_fd_client, &ev) == -1) {
                    Logger::error(strerror(errno));
                    exit(EXIT_FAILURE);
                }

            } else {
                std::string message;
                int fd = events[n].data.fd;

                if (read_data(message, fd)) {
                    write_data(message, fd);
                }
            }
        }
    }

    server.close_server_socket_file_descriptor();
    Logger::footer();
    return 0;
}

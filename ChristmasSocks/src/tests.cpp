#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include <arpa/inet.h>    // inet_ntoa
#include <sys/epoll.h>

#define MAX_EVENTS 10

bool accept_incoming_connection(int &socket_fd_server, struct sockaddr_in &address, int &socket_fd_client) {
    int addrlen = sizeof(address);

    // https://linux.die.net/man/3/accept
    socket_fd_client = accept(
        socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&addrlen
    );

    if (socket_fd_client == -1) {
        Logger::error(strerror(errno));
        return false;
    }

    char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
    Logger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
    Logger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

    return true;
}

bool close_client_socket_file_descriptor(int &socket_fd_client) {
    Logger::info("Closing client socket file descriptor: " + std::to_string(socket_fd_client));

    // https://linux.die.net/man/3/close
    if (close(socket_fd_client) == -1) {
        Logger::error(strerror(errno));
        return false;
    }

    return true;
}

bool read_data(std::string &message, int &socket_fd_client) {
    char buffer[1024] = {0};

    // https://linux.die.net/man/3/read
    int rv = read(socket_fd_client, buffer, 1024);
    message = std::string(buffer);

    if (rv < 0) {
        Logger::error("Socket read() returned error code " + std::to_string(rv));
        return false;
    } else if (rv == 0) {
        Logger::info("Socket read() received EOF - client hang up detected");
        close_client_socket_file_descriptor(socket_fd_client);
        return false;
    } else {
        Logger::info("Read in message '" + message + "' from descriptor " + std::to_string(socket_fd_client));
    }

    return true;
}

bool write_data(std::string &message, int &socket_fd_client) {
    // https://linux.die.net/man/3/write
    int rv = send(socket_fd_client, message.c_str(), message.size(), 0);

    if (rv > -1) {
        Logger::info("Wrote out message '" + message + "' to descriptor " + std::to_string(socket_fd_client));
        return true;
    }

    return false;
}

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
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server.socket_fd_server;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server.socket_fd_server, &ev) == -1) {
        perror("epoll_ctl: server.socket_fd_server");
        exit(EXIT_FAILURE);
    }

    while (true) {

        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
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
                    perror("epoll_ctl: socket_fd_client");
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

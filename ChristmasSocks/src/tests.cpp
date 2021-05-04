#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"

#include <poll.h>
#include <arpa/inet.h>    // inet_ntoa

// continue separating out into member functions
// study the dynamic pollfd loading and make sure this works as intended
// confirm that this matches Beej code
// I'm doing something wrong here!!! poll() is not blocking but immediately returning!!!

class AcceptClients {
    private:
        int fd_count;
        struct sockaddr_in address;
        struct pollfd pfds[5];

    public:
        AcceptClients(int socket_fd_server, struct sockaddr_in address);
        bool accept_incoming_connection();
        bool poll_file_descriptors();
        bool loop();
};

AcceptClients::AcceptClients(int socket_fd_server, struct sockaddr_in address) {
    this->pfds[0].fd = socket_fd_server;
    this->pfds[0].events = POLLIN;  // tell poll to be ready to read on incoming
    this->fd_count = 1;
    this->address = address;
}

bool AcceptClients::accept_incoming_connection() {
    int address_length = sizeof(this->address);

    // https://linux.die.net/man/3/accept
    int socket_fd_client = accept(
        this->pfds[0].fd, (struct sockaddr *)&this->address, (socklen_t*)&address_length
    );

    if (socket_fd_client == -1) {
        if (errno == EBADF) {
            Logger::error("Not a valid file descriptor. Possibly already closed?");
        } else {
            Logger::error(strerror(errno));
        }
        return false;
    }

    char *incoming_ipv4_address = inet_ntoa(this->address.sin_addr);

    Logger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
    Logger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

    this->pfds[this->fd_count].fd = socket_fd_client;
    this->pfds[this->fd_count].events = POLLIN;
    this->fd_count++;

    return true;
}

bool AcceptClients::poll_file_descriptors() {
    // https://man7.org/linux/man-pages/man2/poll.2.html
    int rv = poll(this->pfds, sizeof(this->pfds) / sizeof(struct pollfd), POLL_TIMEOUT_MSEC);

    if (rv > 0) {
        Logger::info("This should wait for time out!");
        return true;
    } else if (rv == 0) {
        Logger::error("System call timed out before any descriptors were read");
        return false;
    } else {
        Logger::error("Failed to poll file descriptors!");
        Logger::error(strerror(errno));
        return false;
    }
}

bool AcceptClients::loop() {
    while(true) {

        if (!this->poll_file_descriptors()) {
            return false;
        }

        for (int i = 0; i < this->fd_count; i++) { // loop over all the file descriptors
            if (this->pfds[i].revents & POLLIN) {  // check if someone is ready to read

                if (this->pfds[i].fd == this->pfds[0].fd) { // i.e. the server fd is ready to read
                    if (!this->accept_incoming_connection()) {
                        continue;
                    }
                } else {
                    char buffer[TCP_BUFFER_SIZE] = {0};

                    // https://linux.die.net/man/3/read
                    int rv = read(this->pfds[i].fd, buffer, TCP_BUFFER_SIZE);

                    if (rv <= 0) {
                        close(this->pfds[i].fd);
                        Logger::info("The kernel has deallocated client socket file descriptor: " + std::to_string(this->pfds[i].fd));
                        this->pfds[i] = this->pfds[this->fd_count - 1];
                        this->fd_count--;
                    } else {
                        std::string message = std::string(buffer);
                        Logger::info("Received " + message);
                        send(this->pfds[i].fd, message.c_str(), message.size(), 0);
                    }
                }
            }
        }
    }

    return true;
}

#define MAX_EVENTS 10
#include <sys/epoll.h>

bool close_client_socket_file_descriptor(int &socket_fd_client) {
    Logger::info("Closing client socket file descriptor: " + std::to_string(socket_fd_client));

    // https://linux.die.net/man/3/close
    if (close(socket_fd_client) == -1) {
        switch (errno) {
            case EBADF:
                Logger::error("Not a valid file descriptor. Possibly already closed?"); break;
            case EINTR:
                Logger::error("Close was interrupted by an IPC signal"); break;
        }
    }
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
    int conn_sock, nfds, epollfd;

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

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        Logger::info("An event has been detected");

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == server.socket_fd_server) {
                int addrlen = sizeof(server.address);

                conn_sock = accept(server.socket_fd_server, (struct sockaddr *) &server.address, (socklen_t*)&addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                //setnonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
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

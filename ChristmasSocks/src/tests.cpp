#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"

#include <poll.h>
#include <arpa/inet.h>    // inet_ntoa

class AcceptClients {
    private:
        int fd_count;
        int socket_fd_server;
        struct sockaddr_in address;
        struct pollfd pfds[5];

    public:
        AcceptClients(int socket_fd_server, struct sockaddr_in address);
        bool accept_incoming_connection();
        bool loop();
};

AcceptClients::AcceptClients(int socket_fd_server, struct sockaddr_in address) {
    this->socket_fd_server = socket_fd_server;
    this->address = address;
}

bool AcceptClients::accept_incoming_connection() {
    int address_length = sizeof(this->address);

    // https://linux.die.net/man/3/accept
    int socket_fd_client = accept(this->socket_fd_server, (struct sockaddr *)&this->address, (socklen_t*)&address_length);

    if (socket_fd_client > -1) {
        char *incoming_ipv4_address = inet_ntoa(this->address.sin_addr);
        Logger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
        Logger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

        this->pfds[this->fd_count].fd = socket_fd_client;
        this->pfds[this->fd_count].events = POLLIN;
        this->fd_count++;

        return true;
    }
    return false;
}

bool AcceptClients::loop() {
    this->pfds[0].fd = this->socket_fd_server;
    this->pfds[0].events = POLLIN;  // tell poll to be ready to read on incoming

    this->fd_count = 1;

    while(true) {
        int poll_response = poll(this->pfds, sizeof(this->pfds) / sizeof(struct pollfd), POLL_TIMEOUT_MSEC);

        if (poll_response < 0) {
            Logger::error("Something is wrong...");
            return false;
        }

        for (int i = 0; i < this->fd_count; i++) { // loop over all the file descriptors
            if (this->pfds[i].revents & POLLIN) {  // check if someone is ready to read

                if (this->pfds[i].fd == this->socket_fd_server) { // i.e. the server fd is ready to read
                    this->accept_incoming_connection();
                } else {
                    char buffer[TCP_BUFFER_SIZE] = {0};

                    // https://linux.die.net/man/3/read
                    int rv = read(this->pfds[i].fd, buffer, TCP_BUFFER_SIZE);

                    if (rv <= 0) {
                        Logger::info("Hang up");
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

int main() {
    Logger::header();
    register_ipc_signals();

    Server server(TCP_PORT, MAX_NUM_CONNECTIONS_QUEUE);
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    AcceptClients client(server.socket_fd_server, server.address);
    client.loop();

    server.close_server_socket_file_descriptor();
    Logger::footer();
    return 0;
}

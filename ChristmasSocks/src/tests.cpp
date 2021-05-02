#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"

#include <poll.h>
#include <arpa/inet.h>    // inet_ntoa

bool wait_for_socket_data(int socket_fd_server, struct sockaddr_in address) {
    if (socket_fd_server < 0) {
        return false;
    }

    struct pollfd pfds[5];

    pfds[0].fd = socket_fd_server;
    pfds[0].events = POLLIN;  // tell poll to be ready to read on incoming

    int fd_count = 1;

    int timeout_msec = 1000; // set to -1 to never time out

    int k = 0;

    while(true) {

        int poll_response = poll(pfds, sizeof(pfds)/sizeof(struct pollfd), timeout_msec);

        if (poll_response < 0) {
            Logger::error("Something is wrong...");
            return false;
        }

        for (int i = 0; i < fd_count; i++) { // loop over all the file descriptors
            if (pfds[i].revents & POLLIN) {  // check if someone is ready to read

                if (pfds[i].fd == socket_fd_server) { // i.e. the server fd is ready to read

                    int address_length = sizeof(address);

                    // https://linux.die.net/man/3/accept
                    int socket_fd_client = accept(socket_fd_server, (struct sockaddr *)&address, (socklen_t*)&address_length);

                    if (socket_fd_client > -1) {
                        char *incoming_ipv4_address = inet_ntoa(address.sin_addr);
                        Logger::info("The kernel has allocated a new client socket file descriptor: " + std::to_string(socket_fd_client));
                        Logger::info("Accepted connection from IPv4 address " + std::string(incoming_ipv4_address));

                        pfds[fd_count].fd = socket_fd_client;
                        pfds[fd_count].events = POLLIN;
                        fd_count++;
                    }
                } else {
                    char buffer[TCP_BUFFER_SIZE] = {0};

                    // https://linux.die.net/man/3/read
                    int rv = read(pfds[i].fd, buffer, TCP_BUFFER_SIZE);

                    if (rv <= 0) {
                        Logger::info("Hang up");
                        close(pfds[i].fd);
                        pfds[i] = pfds[fd_count - 1];
                        fd_count--;
                    } else {
                        std::string message = std::string(buffer);
                        Logger::info("Received " + message);
                        send(pfds[i].fd, message.c_str(), message.size(), 0);
                    }
                    /*
                     * continue somewhere here - something is fucked up
                     */
                }
            }
        }
    }

    return true;
}

int main() {
    Logger::header();
    register_ipc_signals();

    Server server(TCP_PORT, 3);
    server.open_server_socket_file_descriptor();
    server.attach_socket_file_descriptor_to_port();
    server.bind_socket_file_descriptor_to_port();
    server.listen_on_bound_tcp_port();

    wait_for_socket_data(server.socket_fd_server, server.address);

    server.close_server_socket_file_descriptor();
    Logger::footer();
    return 0;
}

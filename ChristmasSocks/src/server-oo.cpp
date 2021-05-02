#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

namespace Logger {
    void info(std::string msg) {
        std::cout << msg << std::endl;
    }

    void error(std::string msg) {
        std::cout << msg << std::endl;
    }
};

class Server {
    private:
        int tcp_port;
        int max_connections_queue;

    public:
        Server(int tcp_port, int max_connections_queue);

        bool open_server_socket_file_descriptor();
        bool close_server_socket_file_descriptor();
        bool attach_socket_file_descriptor_to_port();
        bool bind_socket_file_descriptor_to_port();
        bool listen_on_bound_tcp_port();

        int socket_fd_server;
        struct sockaddr_in address;
};

Server::Server(int tcp_port, int max_connections_queue) {
    this->tcp_port = tcp_port;
    this->max_connections_queue = max_connections_queue;
}

bool Server::open_server_socket_file_descriptor() {
    // https://linux.die.net/man/3/socket
    this->socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);

    if (this->socket_fd_server > -1) {
        Logger::info("The kernel has allocated a new server socket file descriptor " + std::to_string(this->socket_fd_server));
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to open socket file descriptor");

    return false;
}

bool Server::close_server_socket_file_descriptor() {
    Logger::info("Closing server socket file descriptor " + std::to_string(this->socket_fd_server));

    // https://linux.die.net/man/3/close
    if (close(this->socket_fd_server) == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to close socket file descriptor");

    return false;
}

bool Server::attach_socket_file_descriptor_to_port() {
    Logger::info("Attaching socket file descriptor " + std::to_string(this->socket_fd_server) + " to TCP port");

    int optval = 1;

    int rv = setsockopt(
        this->socket_fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)
    );

    if (rv == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to attach socket file descriptor to TCP port");

    return false;
}

bool Server::bind_socket_file_descriptor_to_port() {

    if (this->tcp_port <= 1024) {
        Logger::error("All TCP ports below 1024 are reserved!");
        return false;
    } else if (this->tcp_port > 65535) {
        Logger::error("Cannot bind to a TCP port exceeding 65535!");
        return false;
    } else {
        Logger::info("Binding socket file descriptor " + std::to_string(this->socket_fd_server) + " to TCP port");
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->tcp_port);

    // https://linux.die.net/man/3/bind
    int rv = bind(this->socket_fd_server, (struct sockaddr *)&this->address, sizeof(this->address));

    if (rv == 0) {
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Failed to bind socket file descriptor to TCP port");

    return false;
}

bool Server::listen_on_bound_tcp_port() {
    // https://linux.die.net/man/3/listen
    int rv = listen(this->socket_fd_server, this->max_connections_queue);

    if (rv == 0) {
        Logger::info("Listening on TCP port " + std::to_string(this->tcp_port));
        Logger::info("Will queue a maximum of " + std::to_string(this->max_connections_queue) + " connections");
        return true;
    }

    Logger::error(strerror(errno));
    Logger::error("Cannot listen on TCP port " + std::to_string(this->tcp_port));

    return false;
}

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

    while(k < 25) {
        k++;
        Logger::info("loop counter " + std::to_string(k));
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
                    char buffer[256] = {0};

                    // https://linux.die.net/man/3/read
                    int rv = read(pfds[i].fd, buffer, 256);

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
    Server s(8080, 3);
    s.open_server_socket_file_descriptor();
    s.attach_socket_file_descriptor_to_port();
    s.bind_socket_file_descriptor_to_port();
    s.listen_on_bound_tcp_port();

    wait_for_socket_data(s.socket_fd_server, s.address);
    // pass the s.socket_fd_server to poll() thing here
    // see https://beej.us/guide/bgnet/html/#slightly-advanced-techniques
    // for next steps

    s.close_server_socket_file_descriptor();
    return 0;
}

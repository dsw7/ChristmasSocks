#include "logger.h"

namespace RootLogger {

    void debug(std::string message) {
        if (ENABLE_DEBUG_LOGGING) {
            std::string line = get_current_datetime_string() + " D RT " + message;
            std::cout << line << std::endl;
        }
    }

    void info(std::string message) {
        std::string line = get_current_datetime_string() + " I RT " + message;
        std::cout << line << std::endl;
    }

    void warning(std::string message) {
        std::string line = get_current_datetime_string() + " W RT " + message;
        std::cout << line << std::endl;
    }

    void error(std::string message) {
        std::string line = get_current_datetime_string() + " E RT " + message;
        std::cout << line << std::endl;
    }

    void render_separator() {
        struct winsize window_size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);

        // can also access window_size.ws_row to access rows
        std::string separator(window_size.ws_col, '=');
        std::cout << separator << std::endl;
    }

    void header() {
        render_separator();
        std::cout << "** Project: " << NAME_PROJECT << "\n";
        std::cout << "** Project maintainer: " << MAINTAINER << "\n";
        std::cout << "** Compile date: " << COMPILE_DATE << "\n";
        std::cout << "** Debug logging is disabled by default\n";
        std::cout << "** Legend:\n";
        std::cout << "   - CL: Lines stemming from incoming client connections\n";
        std::cout << "   - SL: Lines stemming from a master server connection\n";
        std::cout << "   - RT: Lines logged from all other parts of the program\n";
        std::cout << std::endl;
        render_separator();
    }

    void footer() {
        render_separator();
        std::cout << "Server shut down at " << get_current_datetime_string() << std::endl;
        render_separator();
    }

}

namespace ClientLogger {

    void debug(std::string message, int &fd) {
        if (ENABLE_DEBUG_LOGGING) {
            std::string line = get_current_datetime_string() + " D CL [" + std::to_string(fd) + "] " + message;
            std::cout << line << std::endl;
        }
    }

    void info(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " I CL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

    void warning(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " W CL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

    void error(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " E CL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

}

namespace ServerLogger {

    void debug(std::string message, int &fd) {
        if (ENABLE_DEBUG_LOGGING) {
            std::string line = get_current_datetime_string() + " D SL [" + std::to_string(fd) + "] " + message;
            std::cout << line << std::endl;
        }
    }

    void info(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " I SL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

    void warning(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " W SL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

    void error(std::string message, int &fd) {
        std::string line = get_current_datetime_string() + " E SL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
    }

}

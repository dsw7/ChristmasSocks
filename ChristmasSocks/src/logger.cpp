#include "logger.h"

namespace Logger {

    void debug(std::string message) {
#if DEBUG_BUILD == 1
        std::string line = get_current_datetime_string() + " D " + message;
        std::cout << line << std::endl;
#endif
    }

    void info(std::string message) {
        std::string line = get_current_datetime_string() + " I " + message;
        std::cout << line << std::endl;
    }

    void warning(std::string message) {
        std::string line = get_current_datetime_string() + " W " + message;
        std::cout << line << std::endl;
    }

    void error(std::string message) {
        std::string line = get_current_datetime_string() + " E " + message;
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
        std::cout << "** Project: " << NAME_PROJECT << std::endl;
        std::cout << "** Project maintainer: " << MAINTAINER << std::endl;
        std::cout << "** Compile date: " << COMPILE_DATE << std::endl;
#if DEBUG_BUILD == 1
        std::cout << "** DEBUG_BUILD enabled - will print all debug log messages!" << std::endl;
#endif
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
#if DEBUG_BUILD == 1
        std::string line = get_current_datetime_string() + " D CL [" + std::to_string(fd) + "] " + message;
        std::cout << line << std::endl;
#endif
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

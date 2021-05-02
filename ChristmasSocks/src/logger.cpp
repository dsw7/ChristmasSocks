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

}

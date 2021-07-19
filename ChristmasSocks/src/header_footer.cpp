#include "logger.h"

void render_separator()
{
    struct winsize window_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
    std::cout << std::string(window_size.ws_col, '=') << std::endl;
}

void display_header()
{
    render_separator();
    std::cout << "** Project: " << NAME_PROJECT << "\n";
    std::cout << "** Project maintainer: " << MAINTAINER << "\n";
    std::cout << "** Compile date: " << COMPILE_DATE << "\n";
    std::cout << "** CMAKE_BUILD_TYPE: " << CMAKE_BUILD_TYPE << "\n";
    std::cout << "** Legend:\n";
    std::cout << "   - CL: Lines stemming from incoming client connections\n";
    std::cout << "   - SL: Lines stemming from a master server connection\n";
    std::cout << "   - RT: Lines logged from all other parts of the program\n";
    std::cout << std::endl;
    render_separator();
}

void display_footer()
{
    render_separator();
    std::cout << "Server shut down at " << get_current_datetime_string() << std::endl;
    render_separator();
}

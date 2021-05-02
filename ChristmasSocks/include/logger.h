#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sys/ioctl.h>  // For ioctl() and TIOCGWINSZ
#include <unistd.h>     // For STDOUT_FILENO
#include "utilities.h"

namespace Logger {
    void debug(std::string);
    void info(std::string);
    void warning(std::string);
    void error(std::string);
    void render_separator();
    void header();
    void footer();
}

#endif

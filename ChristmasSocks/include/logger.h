#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sys/ioctl.h>  // For ioctl() and TIOCGWINSZ
#include <unistd.h>     // For STDOUT_FILENO
#include "utilities.h"
#include "constants.h"

void render_separator();

namespace RootLogger
{
    void info(std::string);
    void warning(std::string);
    void error(std::string);
    void header();
    void footer();
}

namespace ClientLogger
{
    void info(std::string, int &fd);
    void warning(std::string, int &fd);
    void error(std::string, int &fd);
}

namespace ServerLogger
{
    void info(std::string, int &fd);
    void warning(std::string, int &fd);
    void error(std::string, int &fd);
}

#endif

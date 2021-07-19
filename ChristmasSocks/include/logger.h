#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include "utilities.h"

namespace RootLogger
{
    void info(std::string);
    void warning(std::string);
    void error(std::string);
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

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include "utilities.h"

namespace RootLogger
{
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
}

namespace ClientLogger
{
    void info(const std::string &message, int &fd);
    void warning(const std::string &message, int &fd);
    void error(const std::string &message, int &fd);
}

namespace ServerLogger
{
    void info(const std::string &message, int &fd);
    void warning(const std::string &message, int &fd);
    void error(const std::string &message, int &fd);
}

namespace EventLogger
{
    void info(const std::string &message, int &fd);
    void warning(const std::string &message, int &fd);
    void error(const std::string &message, int &fd);
}

#endif

#include "logger.h"

namespace RootLogger {

    void info(const std::string &message)
    {
        std::string line = get_current_datetime_string() + " I RT " + message;
        std::cout << line << std::endl;
    }

    void warning(const std::string &message)
    {
        std::string line = get_current_datetime_string() + " W RT " + message;
        std::cout << line << std::endl;
    }

    void error(const std::string &message)
    {
        std::string line = get_current_datetime_string() + " E RT " + message;
        std::cout << line << std::endl;
    }

}

namespace ClientLogger {

    void info(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " I CL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

    void warning(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " W CL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

    void error(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " E CL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

}

namespace ServerLogger {

    void info(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " I SL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

    void warning(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " W SL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

    void error(const std::string &message, int &fd)
    {
        std::string line = get_current_datetime_string() + " E SL " + std::to_string(fd) + " " + message;
        std::cout << line << std::endl;
    }

}

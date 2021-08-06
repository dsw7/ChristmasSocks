#include "help_menu.h"

void help_message(char *file)
{
    std::cout << "Usage:\n\n";
    std::cout << "  $ " << file;
    std::cout << " [-h]";
    std::cout << " [-p <tcp-port>]";
    std::cout << " [-b <buffer-size>]";
    std::cout << " [-i <bind-ip>]";
    std::cout << " [-n]";
    std::cout << "\n\n";         // add --path-config-file here?
    std::cout << "Options:\n\n";
    std::cout << "  -h, --help                      Print help information and exit\n";
    std::cout << "  -p, --port=<tcp-port>           Specify which TCP port to listen on\n";
    std::cout << "  -b, --buffer-size=<buffer-size> Specify the size of the TCP buffer\n";
    std::cout << "  -i, --bind-ip=<bind-ip>         Specify the bind IP (0.0.0.0 is equivalent to INADDR_ANY)\n";
    std::cout << "  -n, --strip-line-breaks         Strip newlines from incoming client messages\n";
    std::cout << "                                  Useful for manual testing with netcat (nc) \n";
    std::cout << std::endl;
}

bool help_message_lazy_runner(int argc, char **argv)
{
    if (argc < 2)
    {
        return false;
    }

    std::string first_arg = argv[1];

    if (first_arg.compare("-h") == 0)
    {
        help_message(argv[0]);
        return true;
    }
    else if (first_arg.compare("--help") == 0)
    {
        help_message(argv[0]);
        return true;
    }

    return false;
}

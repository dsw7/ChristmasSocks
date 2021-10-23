#include "command_line_interface.h"

void help_general()
{
    std::string general =
    "\e[1m\e[4m--- ChristmasSocks Remote Server Management Software ---\e[0m\n\n"
    "\e[1m\e[4mSynopsis:\e[0m\n\n"
    "  This software is used to manage remote servers used for all DSW testing and development. For\n"
    "  more information about the project and/or licensing information, please visit:\n\n"
    "    \e[4mhttps://github.com/dsw7/ChristmasSocks\e[0m\n";

    std::cout << general << std::endl;
}

void help_usage(char *file)
{
    std::cout << "\e[1m\e[4mUsage:\e[0m\n\n";
    std::cout << "  $ " << file;

    std::string usage =
    " [-h]"
    " [-p <tcp-port>]"
    " [-b <buffer-size>]"
    " [-i <bind-ip>]"
    " [-u <backlog>]"
    " [-w <whitelisted-ip>]"
    "\n\n";

    std::cout << usage;
}

void help_options()
{
    std::string options =
    "\e[1m\e[4mOptions:\e[0m\n\n"
    "  -h, --help                      \e[1mPrint help information and exit.\e[0m\n"
    "  -p, --port=<tcp-port>           \e[1mSpecify which TCP port to listen on.\e[0m This TCP\n"
    "                                  port must not be blocked by a firewall and must not be a\n"
    "                                  a reserved port.\n"
    "  -b, --buffer-size=<buffer-size> \e[1mSpecify the size of the TCP buffer.\e[0m This essentially refers\n"
    "                                  to the maximum size of the message (in bytes) being passed from the\n"
    "                                  master machine to the slave machines.\n"
    "  -i, --bind-ip=<bind-ip>         \e[1mSpecify the bind IP.\e[0m If a value for the bind IP is not passed,\n"
    "                                  the server will default to binding to localhost (127.0.0.1). This\n"
    "                                  is mainly for security reasons and unit testing.\n\n"
    "                                  A live installation should be configured as follows:\n"
    "                                    1. Set --bind-ip=0.0.0.0 (i.e. INADDR_ANY)\n"
    "                                    2. Set \e[1m--whitelist\e[0m=<IPv4 of master machine>\n\n"
    "                                  This configuration will allow the server to accept connections from\n"
    "                                  all interfaces and then block out any addresses not matching the\n"
    "                                  the whitelist address.\n"
    "  -u, --backlog=<backlog>         \e[1mNumber of connections that listener will queue.\e[0m\n"
    "  -w, --whitelist=<ip-addr>       \e[1mAccept connection from this address only.\e[0m. This option\n"
    "                                  is to be used in conjunction with the \e[1m--bind-ip\e[0m option.";

    std::cout << options << std::endl;
}

void help_message(char *file)
{
    help_general();
    help_usage(file);
    help_options();
}

void get_command_line_arguments(int argc, char **argv, Configs &configs)
{
    int option;

    while (1)
    {
        static struct option long_options[] =
        {
            {"help",        no_argument,       0, 'h'},
            {"port",        required_argument, 0, 'p'},
            {"buffer-size", required_argument, 0, 'b'},
            {"bind-ip",     required_argument, 0, 'i'},
            {"backlog",     required_argument, 0, 'u'},
            {"whitelist",   required_argument, 0, 'w'}
        };

        // What's the point of this?
        int option_index = 0;

        /* https://linux.die.net/man/3/getopt_long */
        option = getopt_long(
            argc, argv, "hp:b:i:u:w:", long_options, &option_index
        );

        // End of options
        if (option == -1)
        {
            break;
        }

        switch (option)
        {
            case 'h':
                help_message(argv[0]);
                exit(EXIT_SUCCESS);
            case 'p':
                configs.tcp_port = atoi(optarg);
                break;
            case 'b':
                configs.tcp_buffer_size = atoi(optarg);
                break;
            case 'i':
                configs.bind_ip = optarg;
                break;
            case 'u':
                configs.backlog = atoi(optarg);
                break;
            case 'w':
                configs.whitelist = optarg;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_SUCCESS);
        }
    };
}

#include "command_line_interface.h"

void help_general()
{
    std::string general =
    "\033[1m\033[4m--- ChristmasSocks Remote Server Management Software ---\033[0m\n\n"
    "\033[1mSynopsis:\033[0m\n\n"
    "  This software is used to manage remote servers used for all DSW testing and development. For\n"
    "  more information about the project and/or licensing information, please visit:\n\n"
    "    \033[4mhttps://github.com/dsw7/ChristmasSocks\033[0m\n";

    std::cout << general << std::endl;
}

void help_usage(char *file)
{
    std::cout << "\033[1mUsage:\033[0m\n\n";
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
    "\033[1mOptions:\033[0m\n\n"
    "  -h, --help                      Print help information and exit\n"
    "  -p, --port=<tcp-port>           Specify which TCP port to listen on\n"
    "  -b, --buffer-size=<buffer-size> Specify the size of the TCP buffer\n"
    "  -i, --bind-ip=<bind-ip>         Specify the bind IP (0.0.0.0 is equivalent to INADDR_ANY)\n"
    "  -u, --backlog=<backlog>         Number of connections that listener will queue\n"
    "  -w, --whitelist=<ip-addr>       Accept connection from this IP only\n";

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

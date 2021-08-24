#include "command_line_interface.h"

void help_message(char *file)
{
    std::cout << "Usage:\n\n";
    std::cout << "  $ " << file;
    std::cout << " [-h]";
    std::cout << " [-p <tcp-port>]";
    std::cout << " [-b <buffer-size>]";
    std::cout << " [-i <bind-ip>]";
    std::cout << "\n\n";
    std::cout << "Options:\n\n";
    std::cout << "  -h, --help                      Print help information and exit\n";
    std::cout << "  -p, --port=<tcp-port>           Specify which TCP port to listen on\n";
    std::cout << "  -b, --buffer-size=<buffer-size> Specify the size of the TCP buffer\n";
    std::cout << "  -i, --bind-ip=<bind-ip>         Specify the bind IP (0.0.0.0 is equivalent to INADDR_ANY)\n";
    std::cout << std::endl;
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
            {"bind-ip",     required_argument, 0, 'i'}
        };

        // What's the point of this?
        int option_index = 0;

        /* https://linux.die.net/man/3/getopt_long */
        option = getopt_long(
            argc, argv, "hp:b:i:", long_options, &option_index
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
                exit(EXIT_FAILURE);
            case 'p':
                configs.tcp_port = atoi(optarg);
                break;
            case 'b':
                configs.tcp_buffer_size = atoi(optarg);
                break;
            case 'i':
                configs.bind_ip = optarg;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };
}

#include "command_line_interface.h"

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

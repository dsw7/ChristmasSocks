#include "sysconfig_pipeline.h"

void help_message(char *file)
{
    std::cout << "Usage:\n\n";
    std::cout << "  $ " << file;
    std::cout << " [-h]";
    std::cout << " [-p <tcp-port>]";
    std::cout << " [-b <buffer-size>]\n\n";         // add --path-config-file here?
    std::cout << "Options:\n\n";
    std::cout << "  -h, --help                      Print help information and exit\n";
    std::cout << "  -p, --port=<tcp-port>           Specify which TCP port to listen on\n";
    std::cout << "  -b, --buffer-size=<buffer-size> Specify the size of the TCP buffer\n";
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

void parse_config_file_contents(std::string &file_contents, std::map<std::string, std::string> &raw_configs)
{
    std::istringstream is_file(file_contents);
    std::string line;

    while (std::getline(is_file, line))
    {
        std::istringstream is_line(line);

        std::size_t comment = line.find("#");
        if (comment != std::string::npos)
        {
            // std::string::npos returned if string does not contain #
            continue;
        }

        std::string key;

        if (std::getline(is_line, key, '='))
        {
            std::string value;

            if (std::getline(is_line, value))
            {
                raw_configs[key] = value;
            }
        }
    }
}

void set_root_configs(configs_t &configs)
{
    configs.tcp_port = TCP_PORT;
    configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}

void overwrite_root_configs_with_config_file_configs(configs_t &configs, std::string &path_config_file)
{
    if (!file_exists(path_config_file))
    {
        RootLogger::info("Could not find " + path_config_file);
        return;
    }

    RootLogger::info("Reading configurations from file " + path_config_file);

    std::string file_contents;
    read_file(path_config_file, file_contents);

    std::map<std::string, std::string> raw_configs;
    parse_config_file_contents(file_contents, raw_configs);

    std::map<std::string, std::string>::iterator it;

    for (it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("tcp_port") == 0)
        {
            configs.tcp_port = std::stoi(it->second);
        }
        else if (it->first.compare("tcp_buffer_size") == 0)
        {
            configs.tcp_buffer_size = std::stoi(it->second);
        }
        else if (it->first.compare("max_num_connections_queue") == 0)
        {
            configs.max_num_connections_queue = std::stoi(it->second);
        }
        else
        {
            continue;
        }
    }
}

void overwrite_config_file_configs_with_cli_args(configs_t &global_configs, int argc, char **argv)
{
    int option;

    while (1)
    {
        static struct option long_options[] =
        {
            {"port",        required_argument, 0, 'p'},
            {"buffer_size", required_argument, 0, 'b'}
        };

        // What's the point of this?
        int option_index = 0;

        option = getopt_long(
            argc, argv, "p:b:", long_options, &option_index
        );

        // End of options
        if (option == -1)
        {
            break;
        }

        switch (option)
        {
            case 'p':
                global_configs.tcp_port = atoi(optarg);
                break;
            case 'b':
                global_configs.tcp_buffer_size = atoi(optarg);
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };
}

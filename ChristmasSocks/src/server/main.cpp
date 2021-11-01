#include <iostream>

#include <string>
#include "command_line_interface.h"
#include "parse_config_file.h"
#include "logger.h"
#include "ipc_signal_registers.h"
#include "log_system_configs.h"
#include "header_footer.h"
#include "epoll_server.h"

int main(int argc, char **argv)
{
    Configs configs;

    if (argc == 3 and std::string(argv[1]) == "--config")
    {
        if (!read_configs_from_file(std::string(argv[2]), configs))
        {
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        get_command_line_arguments(argc, argv, configs);
    }

    display_header();
    register_ipc_signals();

    log_system_configs(configs);

    ServerImplMain server(configs);
    server.incoming_client_setup();
    server.server_setup();
    server.open_epoll_file_descriptor();
    server.register_server_fd_to_epoll_instance();
    server.loop();
    server.server_teardown();

    RootLogger::info("Program appears to have exited normally");
    display_footer();
    return 0;
}

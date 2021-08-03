#include <iostream>

#include "help_menu.h"
#include "logger.h"
#include "ipc_signal_registers.h"
#include "system_configurations.h"
#include "header_footer.h"
#include "epoll_server.h"

int main(int argc, char **argv)
{
    if (help_message_lazy_runner(argc, argv))
    {
        std::cout << "Exiting..." << std::endl;
        exit(EXIT_SUCCESS);
    }

    display_header();
    register_ipc_signals();

    SystemConfigurations sysconfig_handle;
    sysconfig_handle.overwrite_root_configs_with_config_file_configs();
    configs_t sysconfigs = sysconfig_handle.overwrite_config_file_configs_with_cli_args(argc, argv);

    ServerImplMain server(sysconfigs);
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

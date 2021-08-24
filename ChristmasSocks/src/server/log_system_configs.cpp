#include "log_system_configs.h"

void log_system_configs(Configs &configs)
{
    RootLogger::info("The following constants will be distributed to the server machinery:");
    RootLogger::info("+ tcp_port =        " + std::to_string(configs.tcp_port));
    RootLogger::info("+ bind_ip  =        " + configs.bind_ip);
    RootLogger::info("+ tcp_buffer_size = " + std::to_string(configs.tcp_buffer_size));
    RootLogger::info("+ backlog =         " + std::to_string(configs.backlog));
}

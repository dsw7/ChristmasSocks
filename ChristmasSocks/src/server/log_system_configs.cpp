#include "log_system_configs.h"

void log_system_configs(configs_t &sysconfigs)
{
    RootLogger::info("The following constants will be distributed to the server machinery:");
    RootLogger::info("+ tcp_port =                  " + std::to_string(sysconfigs.tcp_port));
    RootLogger::info("+ bind_ip  =                  " + sysconfigs.bind_ip);
    RootLogger::info("+ tcp_buffer_size =           " + std::to_string(sysconfigs.tcp_buffer_size));
    RootLogger::info("+ max_num_connections_queue = " + std::to_string(sysconfigs.max_num_connections_queue));
}

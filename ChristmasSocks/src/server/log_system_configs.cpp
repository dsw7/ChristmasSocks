#include "log_system_configs.h"

void log_system_configs(configs_t &sysconfigs)
{
    RootLogger::info("The following constants will be distributed to the server machinery:");
    RootLogger::info("> tcp_port = " + std::to_string(sysconfigs.tcp_port));
}

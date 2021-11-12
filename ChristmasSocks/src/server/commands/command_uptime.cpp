#include "command_uptime.h"

std::string get_hhmmss_from_secs(long &net_seconds)
{
    int net_minutes = net_seconds / 60;
    int net_hours = net_minutes / 60;

    std::string hours = std::to_string(int(net_hours));
    std::string minutes = std::to_string(int(net_minutes % 60));
    std::string seconds = std::to_string(int(net_seconds % 60));

    return hours + ":" + minutes + ";" + seconds;
}

/* https://linux.die.net/man/2/sysinfo */
std::string get_system_uptime()
{
    RootLogger::info("Acquiring operating system uptime");
    struct sysinfo info;

    int rv = sysinfo(&info);
    if (rv == 0)
    {
        return get_hhmmss_from_secs(info.uptime);
    }

    RootLogger::error(strerror(errno));
    return "ERROR";
}

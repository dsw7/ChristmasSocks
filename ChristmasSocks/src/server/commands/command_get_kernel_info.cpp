#include "command_get_kernel_info.h"

std::string command_get_kernel_info()
{
    utsname result;
    uname(&result);

    std::vector<std::string> info;
    info.push_back("Operating system name: " + std::string(result.sysname));
    info.push_back("Node name: " + std::string(result.nodename));
    info.push_back("Operating system release: " + std::string(result.release));
    info.push_back("Operating system version: " + std::string(result.version));
    info.push_back("Hardware identifier: " + std::string(result.machine));

    const char* const delimiter = "\n";

    std::ostringstream joined_info;
    std::copy(info.begin(), info.end(), std::ostream_iterator<std::string>(joined_info, delimiter));

    return joined_info.str();
}

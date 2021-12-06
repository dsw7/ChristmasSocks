#include "command_get_kernel_info.h"

namespace Commands {

    std::string command_get_kernel_info()
    {
        RootLogger::info("Acquiring operating system information");

        utsname result;

        // https://man7.org/linux/man-pages/man2/olduname.2.html
        int rv = uname(&result);

        if (rv != 0)
        {
            std::string error = strerror(errno);
            RootLogger::error("Failed to run command. The error was:");
            RootLogger::error(error);
            return error;
        }

        std::vector<std::string> info;
        info.push_back("System: " + std::string(result.sysname));
        info.push_back("Node: " + std::string(result.nodename));
        info.push_back("Release: " + std::string(result.release));
        info.push_back("Version: " + std::string(result.version));
        info.push_back("Machine: " + std::string(result.machine));

        const char* const delimiter = "\n";

        std::ostringstream joined_info;
        std::copy(info.begin(), info.end(), std::ostream_iterator<std::string>(joined_info, delimiter));

        return joined_info.str();
    }

}

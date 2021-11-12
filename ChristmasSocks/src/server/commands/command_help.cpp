#include "command_help.h"

namespace Commands {

    std::string command_help()
    {
        RootLogger::info("Received help command. Returning a list of internal commands to client");
        return "> List of commands:\n"
        "> help   = Get a list of commands\n"
        "> sleep  = Sleep for a short delay\n"
        "> uptime = Return time since boot in HH:MM:SS format\n"
        "> exit   = Shut down the server\n"
        "> \n"
        "> Unknown commands will simply be echoed back to the client";
    }

}

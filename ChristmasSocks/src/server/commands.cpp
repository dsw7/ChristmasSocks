#include "commands.h"

namespace Commands {

    std::string command_help()
    {
        RootLogger::info("Received help command. Returning a list of internal commands to client");
        return "List of commands:\n"
        "> help  = Get a list of commands\n"
        "> sleep = Sleep for a short delay\n"
        "> exit  = Shut down the server\n"
        "\n"
        "Unknown commands will simply be echoed back to the client";
    }

    void command_sleep()
    {
        RootLogger::info("Received sleep command. Sleeping for " + std::to_string(MAX_SLEEP_DURATION_USEC) + " microseconds");
        usleep(MAX_SLEEP_DURATION_USEC);
    }

}

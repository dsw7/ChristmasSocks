#include "commands.h"

namespace Commands {

    std::string command_help()
    {
        RootLogger::info("Received help command. Returning a list of internal commands to client");
        return "List of commands:\n"
        "help  - Get a list of commands\n"
        "sleep - sleep for a short delay";
    }

    void command_sleep()
    {
        RootLogger::info("Received sleep command. Sleeping for " + std::to_string(MAX_SLEEP_DURATION_USEC) + " microseconds");
        usleep(MAX_SLEEP_DURATION_USEC);
    }

}

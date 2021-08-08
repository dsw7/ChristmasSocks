#include "commands.h"

namespace Commands {
    void command_sleep()
    {
        RootLogger::info("Received sleep command. Sleeping for " + std::to_string(MAX_SLEEP_DURATION_USEC) + " microseconds");
        usleep(MAX_SLEEP_DURATION_USEC);
    }
}

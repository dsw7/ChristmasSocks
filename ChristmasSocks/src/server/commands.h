#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <unistd.h>
#include "logger.h"
#include "constants.h"

namespace Commands {
    std::string command_help();
    void        command_sleep();
}

#endif

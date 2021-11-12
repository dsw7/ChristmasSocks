#ifndef COMMAND_UPDATE_H
#define COMMAND_UPDATE_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/kernel.h> // Access the sysinfo struct
#include <linux/unistd.h> // For making the system calls
#include <sys/sysinfo.h>
#include "logger.h"

std::string get_hhmmss_from_secs(long &seconds);

namespace Commands {
    std::string command_get_system_update();
}

#endif

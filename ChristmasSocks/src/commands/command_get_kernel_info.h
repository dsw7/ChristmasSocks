#ifndef COMMAND_GET_KERNEL_INFO_H
#define COMMAND_GET_KERNEL_INFO_H

#include <string>
#include <vector>
#include <string.h>      // For strerror()
#include <sys/utsname.h> // For utsname struct and uname function
#include <sstream>       // For ostringstream
#include <iterator>      // For ostream_iterator
#include "logger.h"

namespace Commands {
    std::string command_get_kernel_info();
}

#endif

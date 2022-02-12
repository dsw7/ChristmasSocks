#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>
#include "configs.h"
#include "help_messages.h"

void get_command_line_arguments(
    int argc, char **argv, Configs &configs
);

#endif

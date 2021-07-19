#ifndef HEADER_FOOTER_H
#define HEADER_FOOTER_H

#include <iostream>
#include <string>
#include <sys/ioctl.h>  // For ioctl() and TIOCGWINSZ
#include <unistd.h>     // For STDOUT_FILENO
#include "utilities.h"
#include "constants.h"

void render_separator();
void display_header();
void display_footer();

#endif

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// --------------------------------------------------------------
// CONSTANTS TO BE OVERRIDDEN VIA CONFIG FILE OR COMMAND LINE ARGUMENT
// --------------------------------------------------------------
extern unsigned int TCP_PORT;
extern unsigned int MAX_NUM_CONNECTIONS_QUEUE;
extern unsigned int TCP_BUFFER_SIZE;

// --------------------------------------------------------------
// HARD CODED INTERNAL CONSTANTS
// --------------------------------------------------------------
extern std::string CONFIG_FILEPATH;
extern unsigned int MINIMUM_TCP_BUFFER_SIZE;
extern unsigned int MINIMUM_TCP_PORT;
extern unsigned int MAXIMUM_TCP_PORT;

#endif

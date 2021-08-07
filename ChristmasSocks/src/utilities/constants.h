#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// --------------------------------------------------------------
// CONSTANTS TO BE OVERRIDDEN VIA CONFIG FILE OR COMMAND LINE ARGUMENT
// --------------------------------------------------------------
extern unsigned int TCP_PORT;
extern unsigned int MAX_NUM_CONNECTIONS_QUEUE;
extern unsigned int TCP_BUFFER_SIZE;
extern bool         HANDLE_LINE_BREAKS;
extern std::string  DEFAULT_BIND_IP;

// --------------------------------------------------------------
// HARD CODED INTERNAL CONSTANTS
// --------------------------------------------------------------
extern std::string  CONFIG_FILEPATH;
extern unsigned int MINIMUM_TCP_BUFFER_SIZE;
extern unsigned int MINIMUM_TCP_PORT;
extern unsigned int MAXIMUM_TCP_PORT;
extern unsigned int MAX_EPOLL_EVENTS;
extern unsigned int MAX_SLEEP_DURATION_USEC;
extern std::string  BIND_IP_INADDR_ANY;

#endif

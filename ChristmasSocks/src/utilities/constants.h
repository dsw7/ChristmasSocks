#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// --------------------------------------------------------------
// CONSTANTS TO BE OVERRIDDEN VIA CONFIG FILE OR COMMAND LINE ARGUMENT
// --------------------------------------------------------------
extern unsigned int TCP_PORT;
extern unsigned int MAX_NUM_CONNECTIONS_QUEUE;
extern std::string  DEFAULT_BIND_IP;
extern bool         HANDLE_LINE_BREAKS;
extern int          TCP_BUFFER_SIZE;

// --------------------------------------------------------------
// HARD CODED INTERNAL CONSTANTS
// --------------------------------------------------------------
extern std::string  CONFIG_FILEPATH;
extern unsigned int MAXIMUM_TCP_PORT;
extern unsigned int MAX_EPOLL_EVENTS;
extern unsigned int MAX_SLEEP_DURATION_USEC;
extern std::string  BIND_IP_INADDR_ANY;
extern std::string  CHAR_CARRIAGE_RETURN;
extern std::string  CHAR_LINE_FEED;
extern std::string  CHAR_END_OF_LINE;
extern unsigned int MINIMUM_TCP_PORT;
extern int          MINIMUM_TCP_BUFFER_SIZE;

#endif

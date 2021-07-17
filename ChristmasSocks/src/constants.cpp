#include "constants.h"

// Specify path to system configuration file
// This file can be used to override all the following downstream global variables
std::string CONFIG_FILEPATH = "/etc/socks.ini";

// Specify TCP port here
unsigned int TCP_PORT = 8080;

// The maximum number of connections to queue in socket listener (i.e. the 'backlog')
unsigned int MAX_NUM_CONNECTIONS_QUEUE = 5;

// Specify the TCP buffer size for incoming connections (bytes)
unsigned int TCP_BUFFER_SIZE = 1024;

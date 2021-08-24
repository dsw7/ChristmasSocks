#include "constants.h"

// --------------------------------------------------------------
// CONSTANTS TO BE OVERRIDDEN VIA CONFIG FILE OR COMMAND LINE ARGUMENT
// --------------------------------------------------------------

// Specify TCP port here
unsigned int TCP_PORT = 8080;

// The maximum number of connections to queue in socket listener (i.e. the 'backlog')
unsigned int MAX_NUM_CONNECTIONS_QUEUE = 5;

// Specify the TCP buffer size for incoming connections (bytes)
int TCP_BUFFER_SIZE = 1024;

// Specify which host to bind to
std::string DEFAULT_BIND_IP = "127.0.0.1";

// --------------------------------------------------------------
// HARD CODED INTERNAL CONSTANTS
// --------------------------------------------------------------

// Minimum TCP buffer size
int MINIMUM_TCP_BUFFER_SIZE = 16;

// Cutoff for reserved TCP ports
unsigned int MINIMUM_TCP_PORT = 1024;

// Maximum unsigned 16 bit int
unsigned int MAXIMUM_TCP_PORT = 65535;

// Specify the default maximum number of epoll events
// See https://man7.org/linux/man-pages/man2/epoll_wait.2.html (maxevents)
unsigned int MAX_EPOLL_EVENTS = 10;

// Sleep command duration
unsigned int MAX_SLEEP_DURATION_USEC = 1000000;

// Bind to all available interfaces
std::string BIND_IP_INADDR_ANY = "0.0.0.0";

// Accept input from early Mac OSes
std::string CHAR_CARRIAGE_RETURN = "\r";

// Accept input from UNIX based systems (including modern Mac OS)
std::string CHAR_LINE_FEED = "\n";

// Accept input from Windows OSes
std::string CHAR_END_OF_LINE = "\r\n";

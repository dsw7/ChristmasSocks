#include "help_messages.h"

void help_general()
{
    render_separator();
    std::cout << "\e[1m";
    render_centered_text("--- ChristmasSocks Remote Server Management Software ---");
    std::cout << "\e[0m";
    render_separator();

    std::string general =
    "\n\e[1m\e[4mSynopsis:\e[0m\n\n"
    "  This software is used to manage remote servers used for all DSW testing and development. For\n"
    "  more information about the project and/or licensing information, please visit:\n\n"
    "    \e[4mhttps://github.com/dsw7/ChristmasSocks\e[0m\n";

    std::cout << general << std::endl;
}

void help_usage(char *file)
{
    std::cout << "\e[1m\e[4mUsage:\e[0m\n\n";
    std::cout << "  Server can be started up two different ways:\n\n";
    std::cout << "    1. \e[1mManually\e[0m\n";
    std::cout << "    2. \e[1mVia configuration file\e[0m\n\n";

    std::cout << "  1. To start the server manually and override default configurations, pass:\n";
    std::cout << "    $ " << file;
    std::string usage =
    " [-p <tcp-port>]"
    " [-b <buffer-size>]"
    " [-i <bind-ip>]"
    " [-u <backlog>]"
    " [-w <master-ip>]"
    "\n\n";
    std::cout << usage;

    std::cout << "  2. To start the server via configuration, pass:\n";
    std::cout << "    $ " << file << " [--config </path/to/config/file>]\n\n";

    std::string additional_info =
    "  The above two streams are mutually exclusive. That is, a user cannot specify\n"
    "  a path to a configuration file using --config and then override a particular\n"
    "  a parameter defined within the configuration file using a command line option.\n";
    std::cout << additional_info << std::endl;
}

void help_options()
{
    std::string options =
    "\e[1m\e[4mOptions\e[0m [for manually starting server]:\n\n"
    "  -h, --help                      \e[1mPrint help information and exit.\e[0m\n"
    "  -p, --port=<tcp-port>           \e[1mSpecify which TCP port to listen on.\e[0m This TCP\n"
    "                                  port must not be blocked by a firewall and must not be a\n"
    "                                  a reserved port.\n"
    "  -b, --buffer-size=<buffer-size> \e[1mSpecify the size of the TCP buffer.\e[0m This essentially refers\n"
    "                                  to the maximum size of the message (in bytes) being passed from the\n"
    "                                  master machine to the slave machines.\n"
    "  -i, --bind-ip=<bind-ip>         \e[1mSpecify the bind IP.\e[0m If a value for the bind IP is not passed,\n"
    "                                  the server will default to binding to localhost (127.0.0.1). This\n"
    "                                  is mainly for security reasons and unit testing.\n\n"
    "                                  A live installation should be configured as follows:\n"
    "                                    1. Set --bind-ip=0.0.0.0 (i.e. INADDR_ANY)\n"
    "                                    2. Set \e[1m--master\e[0m=<IPv4 of master machine>\n\n"
    "                                  This configuration will allow the server to accept connections from\n"
    "                                  all interfaces and then block out any addresses not matching the\n"
    "                                  the master address.\n"
    "  -u, --backlog=<backlog>         \e[1mNumber of connections that listener will queue.\e[0m\n"
    "  -w, --master=<ip-addr>          \e[1mAccept connection from this address only.\e[0m This option\n"
    "                                  is to be used in conjunction with the \e[1m--bind-ip\e[0m option.\n\n"
    "                                  A live installation should be configured as follows:\n"
    "                                    1. Set \e[1m--bind-ip\e[0m=0.0.0.0 (i.e. INADDR_ANY)\n"
    "                                    2. Set --master=<IPv4 of master machine>";
    std::cout << options << std::endl;
}

void help_message(char *file)
{
    help_general();
    help_usage(file);
    help_options();
}

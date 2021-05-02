#include "ipc_signal_registers.h"

void sigint_handler(int signal) {
    Logger::info("Interrupt signal SIGINT (" + std::to_string(signal) + ") received!");
    Logger::footer();
    exit(signal);
}

void sigterm_handler(int signal) {
    Logger::info("Termination signal SIGTERM (" + std::to_string(signal) + ") received!");
    Logger::footer();
    exit(signal);
}

void register_ipc_signals() {
    std::string pid = std::to_string(getpid());

    Logger::info("Registering IPC signal SIGINT to process " + pid);
    std::signal(SIGINT, sigint_handler);

    Logger::info("Registering IPC signal SIGTERM to process " + pid);
    std::signal(SIGTERM, sigterm_handler);
};

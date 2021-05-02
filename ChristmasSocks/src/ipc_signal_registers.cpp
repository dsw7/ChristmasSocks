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
    Logger::info("Registering IPC signals to process");
    std::signal(SIGINT, sigint_handler);
    std::signal(SIGTERM, sigterm_handler);
};

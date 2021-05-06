#include "config_file.h"

void read_config_file() {
    if (!file_exists(CONFIG_FILEPATH)) {
        RootLogger::info("Could not find " + CONFIG_FILEPATH + ". Using default configurations");
        return;
    }

    // need to put the global var override code here
}

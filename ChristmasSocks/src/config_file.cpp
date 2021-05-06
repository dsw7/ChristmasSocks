#include "config_file.h"

void read_config_file() {
    if (!file_exists(CONFIG_FILEPATH)) {
        RootLogger::info("Could not find " + CONFIG_FILEPATH + ". Using default configurations");
        return;
    }

    /*
     * The solution will be:
     * 1. Read in variables from config file if the file exists
     * 2. Load a struct with the configs
     * 3. If the particular configuration doesn't exist then default to using the extern from constants
     * 4. Pass the struct to class constructors
     */

}

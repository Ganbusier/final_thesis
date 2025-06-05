#ifndef LOGGING_CONFIG_H
#define LOGGING_CONFIG_H

// Define custom logger ID, only used in main_paramSearch.cpp
#define MAIN_PARAM_SEARCH_LOGGER "main_param_search"

#include <easy3d/util/logging.h>

// Function to configure the logging system
inline void configure_logging_for_main_param_search() {
    // First disable all existing loggers
    el::Configurations disableConf;
    disableConf.setToDefault();
    disableConf.setGlobally(el::ConfigurationType::Enabled, "false");
    disableConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    disableConf.setGlobally(el::ConfigurationType::ToFile, "false");
    
    // Apply to all loggers
    el::Loggers::setDefaultConfigurations(disableConf, true);
    
    // Disable Easy3D's logging output
    el::Loggers::reconfigureLogger("default", disableConf);
    
    // Create and configure a logger specifically for main_paramSearch.cpp
    el::Configurations mainConf;
    mainConf.setToDefault();
    mainConf.setGlobally(el::ConfigurationType::Enabled, "true");
    mainConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
    mainConf.setGlobally(el::ConfigurationType::ToFile, "false");
    mainConf.setGlobally(el::ConfigurationType::Format, "%levshort %datetime{%d/%M/%Y %H:%m:%s.%g} [%fbase:%line] %msg");
    
    // Get or create a dedicated logger
    el::Logger* mainLogger = el::Loggers::getLogger(MAIN_PARAM_SEARCH_LOGGER);
    el::Loggers::reconfigureLogger(MAIN_PARAM_SEARCH_LOGGER, mainConf);
}

// Custom LOG macro, only used in main_paramSearch.cpp
#define MAIN_LOG(LEVEL) CLOG(LEVEL, MAIN_PARAM_SEARCH_LOGGER)

#endif // LOGGING_CONFIG_H 
#pragma once

#define CONFIGURATION_ENABLE_LOGGING

#ifdef CONFIGURATION_ENABLE_LOGGING
#include <iostream>
#endif

#ifdef CONFIGURATION_ENABLE_LOGGING
#define APPLICATION_LOG(message) std::cout << message << std::endl;
#else
#define APPLICATION_LOG(message) ;
#endif

#ifdef CONFIGURATION_ENABLE_LOGGING
#define ENABLE_VALIDATION_LAYERS
#endif

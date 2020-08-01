#pragma once

#define CONFIGURATION_ENABLE_LOGGIN

#ifdef CONFIGURATION_ENABLE_LOGGIN
#include <iostream>
#endif

#ifdef CONFIGURATION_ENABLE_LOGGIN
#define APPLICATION_LOG(message) std::cout << message << std::endl;
#else
#define APPLICATION_LOG(message) ;
#endif

#ifdef CONFIGURATION_ENABLE_LOGGIN
#define ENABLE_VALIDATION_LAYERS true
#else
#define ENABLE_VALIDATION_LAYERS false
#endif

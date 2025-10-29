#pragma once
#include <iostream>

#ifdef DEBUG_LOGS
    #define LOG_DEBUG(msg) (std::cerr << "[DEBUG] " << msg << std::endl)
#else
    #define LOG_DEBUG(msg) ((void)0)
#endif

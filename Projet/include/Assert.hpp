//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Logger.hpp"
#include "Macros.hpp"


#if REI_USE_ASSERT
    #if (_MSC_VER && !__INTEL_COMPILER) || (__MINGW32__ || __MINGW64__)
        #define REI_ASSERT(condition, ...) if(!(condition)) { REI_ERROR(__VA_ARGS__); __debugbreak(); }
    #elif _POSIX
        #define REI_ASSERT(condition, ...) if(!(condition)) { REI_ERROR(__VA_ARGS__); std::raise(SIGTRAP) }
    #else
        #define REI_ASSERT(condition, ...) if(!(condition)) { REI_ERROR(__VA_ARGS__); }
    #endif
#else
    #define REI_ASSERT(condition, ...)
#endif

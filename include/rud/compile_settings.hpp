#ifndef RUD_COMPILE_SETTINGS_HPP
#define RUD_COMPILE_SETTINGS_HPP

#include "rud/string.hpp"

#define Assert(expr, msg)\
do {\
    if constexpr (COMPILE_MODE == safe_mode) {\
        _assert(expr, String({const_cast<ascii*>(#expr), sizeof(#expr) - 1}).temp(), msg);\
    }\
} while(0)

#define AlwaysAssert(expr, msg)\
do {\
    _assert(expr, String({const_cast<ascii*>(#expr, sizeof(#expr) - 1}).temp(), msg);\
} while(0)

namespace rud {
    constexpr u32 safe_mode = 1;
    constexpr u32 unsafe_mode = 0;
    
    #ifndef COMPILE_MODE
    #define COMPILE_MODE safe_mode
    #endif

    void _assert(bool value, const String* expr, const String* msg);
}

#endif

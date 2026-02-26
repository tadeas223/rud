#ifndef RUD_BASE_COMPILE_SETTINGS_HPP
#define RUD_BASE_COMPILE_SETTINGS_HPP

#include "rud/base/string.hpp"

#define Assert(expr, msg)\
do {\
    if constexpr (COMPILE_MODE == safer_mode) {\
        _assert(expr, StringView({{const_cast<ascii*>(#expr), sizeof(#expr) - 1}}), msg);\
    }\
} while(0)

#define AlwaysAssert(expr, msg)\
do {\
    _assert(expr, StringView({{const_cast<ascii*>(#expr, sizeof(#expr) - 1}}), msg);\
} while(0)

#define Pre(statement) Assert(statement, Lit("precondition not met"))

            namespace rud {
    constexpr u32 safer_mode = 1;
    constexpr u32 unsafe_mode = 0;
    
    #ifndef COMPILE_MODE
    #define COMPILE_MODE safer_mode
    #endif

    void _assert(bool value, StringView expr, StringView msg);
}

#endif

// #include "rud/os/standard_streams.hpp"
#include "rud/base/string.hpp"
#include <cstdio>
#include "rud/base/compile_settings.hpp"

namespace rud {
    void _assert(bool value, const String expr, const String msg) {
        if(!value) {
            /*
            os::debug_print(Lit("assertion failed\n"));
            os::debug_print(msg);
            os::debug_print(Lit("\n"));
            panic(Lit(""));
            */
            printf("assertion failed\n");
        }
    }
}

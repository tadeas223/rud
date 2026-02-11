#include "rud/os/standard_streams.hpp"
#include "rud/string.hpp"
#include "rud/compile_settings.hpp"

namespace rud {
    void _assert(bool value, const String expr, const String msg) {
        if(!value) {
            os::debug_print(Lit("assertion failed\n"));
            os::debug_print(msg);
            os::debug_print(Lit("\n"));
            panic(Lit(""));
        }
    }
}

#include "rud/base/string.hpp"
#include "rud/base/compile_settings.hpp"
#include "rud/os/std_out.hpp"

using namespace rud::os;

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

#include "rud/base/compile_settings.hpp"

#include "rud/os/platform/std_err.hpp"

using namespace rud::os;

namespace rud {
    void _assert(bool value, StringView expr, StringView msg) {
        if(!value) [[unlikely]] {
            StdErr err;

            err = StdErr::make();
            err.write_str(Lit("assertion failed\n")).or_panic();
            err.write_str(expr).or_panic();
            err.write_str(Lit("\n")).or_panic();
            err.write_str(msg).or_panic();
            err.write_str(Lit("\n")).or_panic();
            panic(Lit(""));
        }
    }
}

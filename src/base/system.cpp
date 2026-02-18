#include "rud/base/system.hpp"

#include "rud/os/platform/system.hpp"
#include "rud/os/platform/std_out.hpp"
#include <unistd.h>

using namespace rud::os;

namespace rud {
    [[noreturn]] void panic(const StringView msg) {
        debug_print(msg);
        debug_print(Lit("program panicked\n%s\n"));

        exit(1); 
    }
}


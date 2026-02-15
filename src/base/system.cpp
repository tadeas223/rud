#include "rud/base/system.hpp"

#include "rud/os_low/system.hpp"
#include "rud/os/std_out.hpp"
#include <unistd.h>

using namespace rud::os;

namespace rud {
    [[noreturn]] void panic(const StringView msg) {
        debug_print(msg);
        debug_print(Lit("program panicked\n%s\n"));

        os_low::exit(1); 
    }
}


#include "rud/base/system.hpp"

#include "rud/os/platform/std_err.hpp"
#include "rud/os/platform/system.hpp"
#include "rud/os/platform/std_out.hpp"
#include <unistd.h>

using namespace rud::os;

namespace rud {
    [[noreturn]] void panic(const StringView msg) {
        StdErr err = StdErr::make();
        err.write_str(msg).try_or([](){
            abort();
        });
        err.write_str(Lit("program panicked\n%s\n")).try_or([](){
            abort();
        });

        abort(); 
    }
}


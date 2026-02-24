#include "rud/base/system.hpp"

#include "rud/os/platform/std_err.hpp"
#include "rud/os/platform/system.hpp"
#include "rud/os/platform/std_out.hpp"
#include <unistd.h>

using namespace rud::os;

namespace rud {
    [[noreturn]] void panic(const StringView msg) {
        StdErr err = StdErr::make();
        err.write_str(msg).or_try([](){
            abort();
        });

        err.write_str(Lit("program panicked\n%s\n")).or_try([](){
            abort();
        });

        abort(); 
    }
    
    [[noreturn]] void panic_raw(const StringView msg) {
        StdErr err = StdErr::make();
        err.write_str(msg).or_try([](){
            abort();
        });

        abort(); 
    }
}


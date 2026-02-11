#include "rud/base/system.hpp"
#include "rud/os_low/system.hpp"
#include "rud/base/memory.hpp"
#include <cstdio>
#include <unistd.h>

namespace rud {
    [[noreturn]] void panic(const String msg) {
        ascii* cstr = msg.to_cstr();
        printf("program panicked\n%s\n", cstr);
        deallocate(cstr);

        os_low::exit(1); 
    }
}


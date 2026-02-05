#include "rud/system.hpp"
#include "rud/ds/linear_view.hpp"
#include "rud/memory.hpp"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace rud {
    [[noreturn]] void panic(const String msg) {
        ascii* cstr = msg.to_cstr();
        printf("program panicked\n%s\n", cstr);
        deallocate(cstr);

        abort(); 
    }
}


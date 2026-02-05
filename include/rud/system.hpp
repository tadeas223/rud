#ifndef RUD_SYSTEM_HPP
#define RUD_SYSTEM_HPP

#include "rud/string.hpp"

namespace rud {
    [[noreturn]] void panic(const String* msg);
    void run_process(const String* bin_path);
}

#endif

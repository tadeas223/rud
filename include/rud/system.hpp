#ifndef RUD_SYSTEM_HPP
#define RUD_SYSTEM_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/string.hpp"

namespace rud {
    [[noreturn]] void panic(const String msg);
}

#endif

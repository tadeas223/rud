#ifndef RUD_BASE_SYSTEM_HPP
#define RUD_BASE_SYSTEM_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/base/string.hpp"

namespace rud {
    [[noreturn]] void panic(const String msg);
}

#endif

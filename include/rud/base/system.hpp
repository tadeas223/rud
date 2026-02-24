#ifndef RUD_BASE_SYSTEM_HPP
#define RUD_BASE_SYSTEM_HPP

#include "rud/base/string.hpp"

namespace rud {
    [[noreturn]] void panic(StringView msg);
    [[noreturn]] void panic_raw(const StringView msg);
}

#endif

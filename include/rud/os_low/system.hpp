#ifndef RUD_OS_LOW_SYSTEM_HPP
#define RUD_OS_LOW_SYSTEM_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/base/string.hpp"
namespace rud::os_low {
    [[noreturn]] void exit(u32 status_code);
    void run_process(StringView msg, const ds::LinearView<StringView> args);
}

#endif

#ifndef RUD_OS_SYSTEM_HPP
#define RUD_OS_SYSTEM_HPP

#include "rud/ds/linear_view.hpp"
#include "rud/string.hpp"
namespace rud::os {
    void run_process(const String msg, const ds::LinearView<String> args);
}

#endif

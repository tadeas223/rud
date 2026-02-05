#ifndef RUD_OS_STDOUT_HPP
#define RUD_OS_STDOUT_HPP

#include "rud/os/io_error.hpp"
#include "rud/result.hpp"
#include "rud/types.hpp"

namespace rud::os {
    struct StdOut {
        static StdOut make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };
}

#endif

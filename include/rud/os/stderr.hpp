#ifndef RUD_OS_STDERR_HPP
#define RUD_OS_STDERR_HPP

#include "rud/os/os_io/io_error.hpp"
#include "rud/result.hpp"
#include "rud/types.hpp"

namespace rud::os {
    struct StdErr {
        static StdErr make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };
}

#endif

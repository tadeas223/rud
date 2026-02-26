#include "rud/os/platform/std_out.hpp"
#include <unistd.h>

namespace rud::os {
    Result<u64, IOError> StdOut::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(1, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
}


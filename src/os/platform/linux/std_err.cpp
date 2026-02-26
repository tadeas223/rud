#include "rud/os/platform/std_err.hpp"
#include <unistd.h>

namespace rud::os {
    Result<u64, IOError> StdErr::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(2, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
}


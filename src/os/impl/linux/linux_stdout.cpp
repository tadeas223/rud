#include "rud/os/stdout.hpp"
#include <unistd.h>

using namespace rud;

namespace rud::os {
    StdOut StdOut::make() {
        return {};          
    }

    Result<u64, IOError> StdOut::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(1, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> StdOut::write(const String str) {
        return write(str.chars, str.len); 
    }

}

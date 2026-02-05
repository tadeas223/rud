#include "rud/os/io.hpp"
#include <unistd.h>

using namespace rud::os;
using namespace rud;

namespace rud::os {
    StdErr StdErr::make() {
        return {};          
    }

    Result<u64, IOError> StdErr::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(2, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> StdErr::write(const String str) {
        return write(str.chars, str.len); 
    }
}

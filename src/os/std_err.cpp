#include "rud/os/std_err.hpp"
#include "rud/os_low/io.hpp"

using namespace rud::os_low;

namespace rud::os {

    StdErr StdErr::make() {
        return {.p_handle = std_err_handle };
    }
    
    Result<u64, os_low::IOError> StdErr::write(const void* buffer, u64 size) {
        return std_stream_handle_write(&p_handle, buffer, size);
    }
}


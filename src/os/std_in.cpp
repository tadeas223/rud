#include "rud/os/std_in.hpp"

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/os_low/io_error.hpp"

namespace rud::os {
        StdIn StdIn::make() {
            return {.p_handle = os_low::std_in_handle };
        }

    Result<u64, os_low::IOError> StdIn::read(void* buffer, u64 size) {
        return os_low::std_stream_handle_read(&p_handle, buffer, size);
    }

}

#include "rud/os/std_out.hpp"
#include "rud/os_low/io.hpp"

using namespace rud::os_low;

namespace rud::os {

    StdOut StdOut::make() {
        return {.p_handle = std_out_handle };
    }
    
    Result<u64, os_low::IOError> StdOut::write(const void* buffer, u64 size) {
        return std_stream_handle_write(&p_handle, buffer, size);
    }

    void debug_print(String str) {
        StdOut::make().write_str(str).or_panic();
    }
}


#ifndef RUD_OS_STD_OUT_HPP
#define RUD_OS_STD_OUT_HPP

#include "rud/os/write_stream.hpp"
#include "rud/os_low/io.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdOut  : WriteStream<StdOut, os_low::IOError> {
        os_low::StdStreamHandle p_handle;

        static StdOut make();       
        
        Result<u64, os_low::IOError> write(const void* buffer, u64 size);
    };

    void debug_print(StringView str);
}

#endif

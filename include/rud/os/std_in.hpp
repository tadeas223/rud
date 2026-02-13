#ifndef RUD_OS_STD_IN_HPP
#define RUD_OS_STD_IN_HPP

#include "rud/os/read_stream.hpp"
#include "rud/os_low/io.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdIn : ReadStream<StdIn, os_low::IOError> {
        os_low::StdStreamHandle p_handle;

        static StdIn make();       
        Result<u64, os_low::IOError> read(void* buffer, u64 size);
        Result<u64, os_low::IOError> write(const void* buffer, u64 size);
        void destroy();
    };
}

#endif

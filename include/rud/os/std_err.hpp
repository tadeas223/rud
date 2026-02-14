#ifndef RUD_OS_STD_ERR_HPP
#define RUD_OS_STD_ERR_HPP

#include "rud/os/write_stream.hpp"
#include "rud/os_low/io.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdErr  : WriteStream<StdErr, os_low::IOError> {
        os_low::StdStreamHandle p_handle;

        static StdErr make();       
        
        Result<u64, os_low::IOError> write(const void* buffer, u64 size);
    };
}

#endif

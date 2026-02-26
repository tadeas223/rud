#ifndef RUD_OS_PLATFORM_STD_ERR_HPP
#define RUD_OS_PLATFORM_STD_ERR_HPP

#include "rud/os/core/io_error.hpp"
#include "rud/os/core/write_stream.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdErr  : WriteStream<StdErr, IOError> {
        static StdErr make() {
            return {};
        }

        Result<u64, IOError> write(const void* buffer, u64 size);
    };
}

#endif

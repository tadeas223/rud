#ifndef RUD_OS_PLATFORM_STD_IN_HPP
#define RUD_OS_PLATFORM_STD_IN_HPP

#include "rud/os/core/read_stream.hpp"
#include "rud/os/core/io_error.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdIn : ReadStream<StdIn, IOError> {
        inline static StdIn make() {
            return {};
        }

        Result<u64, IOError> read(void* buffer, u64 size);
    };
}

#endif

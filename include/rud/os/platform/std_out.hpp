#ifndef RUD_OS_PLATFORM_STD_OUT_HPP
#define RUD_OS_PLATFORM_STD_OUT_HPP

#include "rud/os/core/io_error.hpp"
#include "rud/os/core/write_stream.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct StdOut  : WriteStream<StdOut, IOError> {
        inline static StdOut make() {
            return {};
        }

        Result<u64, IOError> write(const void* buffer, u64 size);
    };

    inline void debug_print(StringView str) {
        StdOut out = StdOut::make();
        out.write_str(str)
            .except(Lit("failed to write to standard out"));
    }
}

#endif

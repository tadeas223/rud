#ifndef OS_STANDARD_STREAMS_HPP
#define OS_STANDARD_STREAMS_HPP

#include "rud/os/io_error.hpp"
#include "rud/result.hpp"
#include "rud/types.hpp"

namespace rud::os {
    struct StdOut {
        static StdOut make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };

    struct StdErr {
        static StdErr make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };

    struct StdIn {
         
    };
    
    inline void debug_print(const String str) {
        StdOut out = StdOut::make();
        out.write(str).except(Lit("stdout closed"));
    }
}

#endif

#ifndef RUD_STREAMS_HPP
#define RUD_STREAMS_HPP

#include "rud/result.hpp"
#include "rud/types.hpp"
#include "rud/os/io_error.hpp"

namespace rud {
    template<typename Derived>
    struct ReadStream {
        inline Result<u64, os::IOError> read(void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->read(buffer, size);
        }
        
    };
    
    template<typename Derived>
    struct WriteStream {
        inline Result<u64, os::IOError> write(const void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->write(buffer, size);
        }
        
    };
}

#endif

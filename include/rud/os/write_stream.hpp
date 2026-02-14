#ifndef RUD_OS_WRITE_STREAM_HPP
#define RUD_OS_WRITE_STREAM_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/vector.hpp"
#include "rud/ds/array.hpp"
#include "rud/os_low/io_error.hpp"

namespace rud::os {
    template<typename Derived, typename ErrType>
    struct WriteStream {
        inline Result<u64, ErrType> write(const void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->write(buffer, size);
        }

        inline Result<void, ErrType> write_str(String str) {
            Result<u64, ErrType> r_written = write(str.chars, str.len);
            if(r_written.is_error()) {
                return Result<void, ErrType>::make_error(r_written.unwrap_error());
            }
            
            return Result<void, ErrType>::make_ok();
        }
    };
}

#endif

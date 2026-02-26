#ifndef RUD_OS_CORE_WRITE_STREAM_HPP
#define RUD_OS_CORE_WRITE_STREAM_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"

namespace rud::os {
    template<typename Derived, typename ErrType>
    struct WriteStream {
        inline Result<u64, ErrType> write(const void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->write(buffer, size);
        }

        inline Result<void, ErrType> write_str(StringView str) {
            Result<u64, ErrType> r_written = write(str.chars, str.len);
            if(!r_written.is_ok()) {
                return Result<void, ErrType>::make_error(r_written.unwrap_error());
            }
            
            return Result<void, ErrType>::make_ok();
        }
    };
}

#endif

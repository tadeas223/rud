#ifndef RUD_OS_READ_STREAM_HPP
#define RUD_OS_READ_STREAM_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/os_low/io_error.hpp"
namespace rud::os {
    template<typename Derived>
    struct ReadStream {
        inline Result<u64, os_low::IOError> read(void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->read(buffer, size);
        }

        AllocString read_all() {
        }
        
        AllocString read_until(u8 sepparator) {
        }

        Result<u8, os_low::IOError> read_byte() {
            u8 value;

            Result<u64, os_low::IOError> result = read(&value, 1);
            if(result.is_error()) {
                return Result<u8, os_low::IOError>::make_error(result.unwrap_error());
            }
            
            if(result.unwrap() != 1) {
                return Result<u8, os_low::IOError>::make_error(os_low::IOError::Other);
            }

            return Result<u8, os_low::IOError>::make_ok(value);
        }
    };
}

#endif

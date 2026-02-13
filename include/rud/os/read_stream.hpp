#ifndef RUD_OS_READ_STREAM_HPP
#define RUD_OS_READ_STREAM_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/vector.hpp"
#include "rud/ds/array.hpp"
#include "rud/os_low/io_error.hpp"
namespace rud::os {
    template<typename Derived>
    struct ReadStream {
        inline Result<u64, os_low::IOError> read(void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->read(buffer, size);
        }

        // Result<AllocString, os_low::IOError> read_all() {
        // }
        
        Result<AllocString, os_low::IOError> read_line() {
            return read_until('\n');
        }
        
        Result<AllocString, os_low::IOError> read_until(u8 sepparator) {
            ds::Vector<ascii> vec = ds::Vector<ascii>::make(512);
            
            ds::Array<ascii, 512> buffer = ds::Array<ascii, 512>();
            
            bool done = false;
            while(!done) {
                Result<u64, os_low::IOError> result = read(buffer.data(), buffer.len());
                if(result.is_error()) {
                    return Result<AllocString, os_low::IOError>::make_error(result.unwrap_error());
                }
                u32 buffer_len = result.unwrap();
                
                for(u32 i = 0; i < buffer_len; i++) {
                    if(*buffer[i] != sepparator) {
                        vec.push(*buffer[i]);
                    } else {
                        done = true;
                        break;
                    }
                }
            }
            
            u32 len = vec.len();
            AllocString str = AllocString::make_take(vec.destroy_to_array(), len);
            return Result<AllocString, os_low::IOError>::make_ok(str);
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

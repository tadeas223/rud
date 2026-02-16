#ifndef RUD_OS_READ_STREAM_HPP
#define RUD_OS_READ_STREAM_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/ds/c_vector.hpp"
#include "rud/ds/array.hpp"
#include "rud/os_low/io_error.hpp"

namespace rud::os {
    template<typename Derived, typename ErrType>
    struct ReadStream {
        inline Result<u64, ErrType> read(void* buffer, u64 size) {
            return reinterpret_cast<Derived*>(this)->read(buffer, size);
        }

        Result<C_StringAlloc, ErrType> read_line() {
            return read_until('\n');
        }
        
        Result<C_StringAlloc, ErrType> read_until(u8 sepparator) {
            ds::C_DArray<ascii> vec = ds::C_DArray<ascii>::make(512);

            ds::Array<ascii, 512> buffer = ds::Array<ascii, 512>();
            
            bool done = false;
            while(!done) {
                Result<u64, ErrType> result = read(buffer.data, buffer.len());
                if(!result.ok) {
                    return Result<C_StringAlloc, ErrType>::make_error(result.unwrap_error());
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
            
            u32 len = vec.len;
            C_StringAlloc str = C_StringAlloc::make_take(vec.destroy_to_array(), len);
            return Result<C_StringAlloc, ErrType>::make_ok(str);
        }

        Result<u8, ErrType> read_byte() {
            u8 value;

            Result<u64, ErrType> result = read(&value, 1);
            if(!result.ok) {
                return Result<u8, ErrType>::make_error(result.unwrap_error());
            }
            
            if(result.unwrap() != 1) {
                return Result<u8, ErrType>::make_error(os_low::IOError::Other);
            }

            return Result<u8, ErrType>::make_ok(value);
        }
    };
}

#endif

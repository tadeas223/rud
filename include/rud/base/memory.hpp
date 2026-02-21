#ifndef RUD_BASE_MEMORY_HPP
#define RUD_BASE_MEMORY_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"

namespace rud {
    enum class AllocError {
        OutOfMemory, 
    };
    
    // may throw AllocError
    // never returns null
    void* allocate_size(u64 size);
    
    Result<void*, AllocError> try_allocate_size(u64 size);
    
    template<typename T>
    inline Result<T*, AllocError> try_allocate(T original) {
        Result<void*, AllocError> r_ptr = try_allocate_size(sizeof(T));
        if(r_ptr.ok) {
            T* ptr = static_cast<T*>(r_ptr.unwrap());
            *ptr = original;
            return Result<T*, AllocError>::make_ok(ptr);
        } else {
            return Result<T*, AllocError>::make_error(r_ptr.unwrap_error());
        }
        

    }
  
    template<typename T>
    inline T* allocate(T original) {
        T* ptr = static_cast<T*>(allocate_size(sizeof(T)));
        *ptr = original;
        return ptr;
    }
    
    void* reallocate(void* ptr, u64 new_size);
    Result<void*, AllocError> try_reallocate(void* ptr, u64 new_size);
    void deallocate(void* ptr);

    void mem_copy(void* dest, const void* src, u64 size);
    void mem_move(void* dest, const void* src, u64 size);
    void mem_set(void* dest, u8 value, u64 size);
    bool mem_equals(const void* ptr1, const void* ptr2, u64 size);

    u64 mem_align_forward(u64 value, u64 align);
}

#endif

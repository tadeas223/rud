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
        Result<T*, AllocError> r_ptr = static_cast<T*>(try_allocate_size(sizeof(T)));
        if(r_ptr.is_ok()) {
            *r_ptr.unwrap() = original;
        }

        return r_ptr;
    }
  
    template<typename T>
    inline T* allocate(T original) {
        T* ptr = static_cast<T*>(allocate_size(sizeof(T)));
        *ptr = original;
        return ptr;
    }
    
    void deallocate(void* ptr);

    void mem_copy(void* dest, const void* src, u64 size);
    void mem_move(void* dest, const void* src, u64 size);
    void mem_set(void* dest, const u8 value, u64 size);
    bool mem_equals(const void* ptr1, const void* ptr2, u64 size);
}

#endif

#include "rud/base/memory.hpp"

#include "rud/base/compile_settings.hpp"
#include "rud/base/result.hpp"
#include "rud/base/env.hpp"
#include <cstdlib>
#include <cstring>

namespace rud {
    Result<void*, AllocError> try_allocate_size(u64 size) {
        Assert(size != 0, Lit("cannot allocate memory of size 0"));

        void* ptr = malloc(size);
        if(ptr == nullptr) {
            return Result<void*, AllocError>::make_error(AllocError::OutOfMemory); 
        }

        return Result<void*, AllocError>::make_ok(ptr);
    }
    
    void* allocate_size(u64 size) {
        Assert(size != 0, Lit("cannot allocate memory of size 0"));
        
        void* ptr = malloc(size);
        if(ptr == nullptr) {
            #ifdef EXCEPTIONS_ENABLED
            throw AllocError::OutOfMemory;
            #else
            panic(Lit("allocation failed"));
            #endif
        }

        return ptr;
    }
    
    Result<void*, AllocError> try_reallocate(void* ptr, u64 new_size) {
        Assert(new_size != 0, Lit("cannot allocate memory of size 0"));
        
        void* new_ptr = realloc(ptr, new_size);
        if(new_ptr == nullptr) {
            return Result<void*, AllocError>::make_error(AllocError::OutOfMemory); 
        }
        return Result<void*, AllocError>::make_ok(new_ptr);
    }

    void* reallocate(void* ptr, u64 new_size) {
        Assert(new_size != 0, Lit("cannot allocate memory of size 0"));
        
        void* new_ptr = realloc(ptr, new_size);
        if(new_ptr == nullptr) {
            #ifdef EXCEPTIONS_ENABLED
            throw AllocError::OutOfMemory;
            #else
            panic(Lit("allocation failed"));
            #endif
        }

        return new_ptr;
    }

    void deallocate(void* ptr) {
        free(ptr); 
    }
    
    void mem_copy(void* dest, const void* src, u64 size) {
        while(size--) {
            reinterpret_cast<u8*>(dest)[size] = reinterpret_cast<const u8*>(src)[size];
        }
    }

    void mem_move(void* dest, const void* src, u64 size) {
        if(dest > src) {
            while(size--) {
                reinterpret_cast<u8*>(dest)[size] = reinterpret_cast<const u8*>(src)[size];
            }
        } else {
            for(u32 i = 0; i < size; ++i) {
                reinterpret_cast<u8*>(dest)[i] = reinterpret_cast<const u8*>(src)[i];
            }
        }
    }

    void mem_set(void* dest, const u32 value, u64 size) {
        memset(dest, value, size);
    }

    bool mem_equals(const void* ptr1, const void* ptr2, u64 size) {
        return memcmp(ptr1, ptr2, size); 
    }
}

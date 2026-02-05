#include "rud/memory.hpp"
#include "rud/compile_settings.hpp"
#include "rud/result.hpp"
#include "rud/env.hpp"
#include <cstdlib>
#include <cstring>

namespace rud {
    Result<void*, AllocError> try_allocate_size(u64 size) {
        Assert(size != 0, Lit("cannot allocate memory of size 0").temp());

        void* ptr = malloc(size);
        if(ptr == nullptr) {
            return Result<void*, AllocError>::make_error(AllocError::OutOfMemory); 
        }

        return Result<void*, AllocError>::make_ok(ptr);
    }
    
    void* allocate_size(u64 size) {
        Assert(size != 0, Lit("cannot allocate memory of size 0").temp());
        
        void* ptr = malloc(size);
        if(ptr == nullptr) {
            #ifdef EXCEPTIONS_ENABLED
            throw AllocError::OutOfMemory;
            #else
            panic(Lit("allocation failed").temp());
            #endif
        }

        return ptr;
    }

    void deallocate(void* ptr) {
        free(ptr); 
    }

    void mem_copy(void* dest, const void* src, u64 size) {
        memcpy(dest, src, size);
    }

    void mem_move(void* dest, const void* src, u64 size) {
        memmove(dest, src, size);
    }

    void mem_set(void* dest, const u32 value, u64 size) {
        memset(dest, value, size);
    }

    bool mem_equals(const void* ptr1, const void* ptr2, u64 size) {
        return memcmp(ptr1, ptr2, size); 
    }
}

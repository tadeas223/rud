#include "rud/os/platform/memory.hpp"
#include <unistd.h>

#include <sys/mman.h>

namespace rud::os {

    u64 mem_page() {
        static u64 page_size = 0;
        if(page_size == 0) {
            page_size = getpagesize();
        }
        return page_size;
    }

    Result<void*, OsAllocError> mem_reserve(u64 size) {
        if(size % mem_page() != 0) {
            return Result<void*, OsAllocError>::make_error(OsAllocError::Other);
        }

        void* mmap_result = mmap(nullptr, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, 0, 0);
        if(mmap_result == MAP_FAILED) {
            return Result<void*, OsAllocError>::make_error(OsAllocError::Other);
        }

        return Result<void*, OsAllocError>::make_ok(mmap_result);
    }

    Result<void, OsAllocError> mem_commit(void* ptr, u64 size) {
        if(size % mem_page() != 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        int prot_result = mprotect(ptr, size, PROT_WRITE|PROT_READ);
        if(prot_result < 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        return Result<void, OsAllocError>::make_ok();
    }

    Result<void, OsAllocError> mem_decommit(void* ptr, u64 size) {
        if(size % mem_page() != 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        int prot_result = mprotect(ptr, size, PROT_NONE);
        if(prot_result < 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        return Result<void, OsAllocError>::make_ok();
    }

    Result<void, OsAllocError> mem_release(void* ptr, u64 size) {
        if(size % mem_page() != 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        int munmap_result = munmap(ptr, size);
        if(munmap_result < 0) {
            return Result<void, OsAllocError>::make_error(OsAllocError::Other);
        }

        return Result<void, OsAllocError>::make_ok();
    }

}

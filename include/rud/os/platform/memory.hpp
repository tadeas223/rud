#ifndef RUD_OS_PLATFORM_MEMORY_HPP
#define RUD_OS_PLATFORM_MEMORY_HPP

#include "rud/base/result.hpp"
#include "rud/base/types.hpp"
#include "rud/os/core/os_alloc_error.hpp"

namespace rud::os {
    u64 mem_page();
    Result<void*, OsAllocError> mem_reserve(u64 size);
    Result<void, OsAllocError> mem_commit(void* ptr, u64 size);
    Result<void, OsAllocError> mem_decommit(void* ptr, u64 size);
    Result<void, OsAllocError> mem_release(void* ptr, u64 size);
}

#endif

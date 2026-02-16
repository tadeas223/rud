#ifndef RUD_ARCH_LOW_MEMORY_HPP
#define RUD_ARCH_LOW_MEMORY_HPP

#include "rud/base/types.hpp"

namespace rud::arch_low {
    void mem_copy(void* dest, const void* src, u64 size);

    void mem_move(void* dest, const void* src, u64 size);
}

#endif

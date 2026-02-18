#include <gtest/gtest.h>
#include "rud/base/memory.hpp"
#include "rud/base/types.hpp"

using namespace rud;

//
// allocation basics
//

TEST(Memory, AllocateSizeReturnsValidPointer) {
    for (u64 size = 1; size <= 1024; size *= 2) {
        void* ptr = allocate_size(size);
        ASSERT_NE(ptr, nullptr);
        deallocate(ptr);
    }
}

TEST(Memory, TryAllocateSizeSucceedsForSmallSizes) {
    for (u64 size = 1; size <= 1024; size *= 2) {
        auto result = try_allocate_size(size);
        ASSERT_TRUE(result.ok);
        ASSERT_NE(result.unwrap(), nullptr);
        deallocate(result.unwrap());
    }
}

//
// typed allocation
//

struct TestStruct {
    u32 a;
    u32 b;
};

TEST(Memory, AllocateCopiesValue) {
    TestStruct original{42, 77};

    TestStruct* ptr = allocate(original);

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->a, 42u);
    EXPECT_EQ(ptr->b, 77u);

    deallocate(ptr);
}

TEST(Memory, TryAllocateCopiesValue) {
    for (u32 i = 0; i < 128; ++i) {
        TestStruct original{i, i * 3};

        auto result = try_allocate(original);
        ASSERT_TRUE(result.ok);

        TestStruct* ptr = result.unwrap();
        EXPECT_EQ(ptr->a, i);
        EXPECT_EQ(ptr->b, i * 3);

        deallocate(ptr);
    }
}

//
// reallocation
//

TEST(Memory, ReallocatePreservesData) {
    const u64 initial_size = 64;
    const u64 new_size = 256;

    u8* ptr = static_cast<u8*>(allocate_size(initial_size));
    ASSERT_NE(ptr, nullptr);

    for (u64 i = 0; i < initial_size; ++i)
        ptr[i] = static_cast<u8>(i);

    ptr = static_cast<u8*>(reallocate(ptr, new_size));
    ASSERT_NE(ptr, nullptr);

    for (u64 i = 0; i < initial_size; ++i)
        EXPECT_EQ(ptr[i], static_cast<u8>(i));

    deallocate(ptr);
}

TEST(Memory, TryReallocatePreservesData) {
    const u64 initial_size = 128;
    const u64 new_size = 512;

    u8* ptr = static_cast<u8*>(allocate_size(initial_size));

    for (u64 i = 0; i < initial_size; ++i)
        ptr[i] = static_cast<u8>(i + 1);

    auto result = try_reallocate(ptr, new_size);
    ASSERT_TRUE(result.ok);

    ptr = static_cast<u8*>(result.unwrap());

    for (u64 i = 0; i < initial_size; ++i)
        EXPECT_EQ(ptr[i], static_cast<u8>(i + 1));

    deallocate(ptr);
}

//
// mem_copy
//

TEST(Memory, MemCopyCopiesExactData) {
    constexpr u64 size = 256;

    u8* src  = static_cast<u8*>(allocate_size(size));
    u8* dest = static_cast<u8*>(allocate_size(size));

    for (u64 i = 0; i < size; ++i)
        src[i] = static_cast<u8>(i);

    mem_copy(dest, src, size);

    for (u64 i = 0; i < size; ++i)
        EXPECT_EQ(dest[i], src[i]);

    deallocate(src);
    deallocate(dest);
}

//
// mem_move
//

TEST(Memory, MemMoveHandlesOverlapForward) {
    constexpr u64 size = 128;

    u8* buffer = static_cast<u8*>(allocate_size(size));

    for (u64 i = 0; i < size; ++i)
        buffer[i] = static_cast<u8>(i);

    mem_move(buffer + 10, buffer, size - 10);

    for (u64 i = 0; i < size - 10; ++i)
        EXPECT_EQ(buffer[i + 10], static_cast<u8>(i));

    deallocate(buffer);
}

TEST(Memory, MemMoveHandlesOverlapBackward) {
    constexpr u64 size = 128;

    u8* buffer = static_cast<u8*>(allocate_size(size));

    for (u64 i = 0; i < size; ++i)
        buffer[i] = static_cast<u8>(i);

    mem_move(buffer, buffer + 10, size - 10);

    for (u64 i = 0; i < size - 10; ++i)
        EXPECT_EQ(buffer[i], static_cast<u8>(i + 10));

    deallocate(buffer);
}

//
// mem_set
//

TEST(Memory, MemSetFillsMemory) {
    constexpr u64 size = 256;

    u8* buffer = static_cast<u8*>(allocate_size(size));

    for (u8 value = 0; value < 10; ++value) {
        mem_set(buffer, value, size);

        for (u64 i = 0; i < size; ++i)
            EXPECT_EQ(buffer[i], value);
    }

    deallocate(buffer);
}

//
// mem_equals
//

TEST(Memory, MemEqualsReturnsTrueForEqualBuffers) {
    constexpr u64 size = 256;

    u8* a = static_cast<u8*>(allocate_size(size));
    u8* b = static_cast<u8*>(allocate_size(size));

    for (u64 i = 0; i < size; ++i) {
        a[i] = static_cast<u8>(i);
        b[i] = static_cast<u8>(i);
    }

    EXPECT_TRUE(mem_equals(a, b, size));

    deallocate(a);
    deallocate(b);
}

TEST(Memory, MemEqualsDetectsDifference) {
    constexpr u64 size = 256;

    u8* a = static_cast<u8*>(allocate_size(size));
    u8* b = static_cast<u8*>(allocate_size(size));

    for (u64 i = 0; i < size; ++i) {
        a[i] = static_cast<u8>(i);
        b[i] = static_cast<u8>(i);
    }

    for (u64 i = 0; i < size; ++i) {
        b[i] ^= 0xFF;
        EXPECT_FALSE(mem_equals(a, b, size));
        b[i] ^= 0xFF;
    }

    deallocate(a);
    deallocate(b);
}


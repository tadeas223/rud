#include "gtest/gtest.h"

#include "rud/base/memory.hpp"
#include "helpers.hpp"
#include "rud/base/types.hpp"
#include <limits>

using namespace rud;

namespace {
    class memory_fixture: public ::testing::TestWithParam<u32>{};
    
    TEST_P(memory_fixture, try_allocate_size) {
        u32 bytes = GetParam();
        
        Result<void*, AllocError> result = try_allocate_size(bytes);
    
        EXPECT_TRUE(result.is_ok());
        EXPECT_TRUE(result.unwrap() != nullptr);
        
        deallocate(result.unwrap());
    }
    
    TEST(memory, try_allocate_size_too_big) {
        constexpr u64 bytes = std::numeric_limits<u64>::max();
    
        Result<void*, AllocError> result = try_allocate_size(bytes);
    
        EXPECT_TRUE(result.is_error());
    }

    TEST_P(memory_fixture, allocate_size) {
        u64 bytes = GetParam();
    
        void* result = nullptr;
        EXPECT_NO_THROW({ result = allocate_size(bytes); });
        
        EXPECT_NE(result, nullptr);
        
        deallocate(result);
    }

    TEST(memory, allocate_size_too_big) {
        u64 bytes = std::numeric_limits<u64>::max();
    
        EXPECT_THROW(allocate_size(bytes), AllocError);
    }

    TEST(memory, try_allocate) {
        constexpr u32 integer = 10;
    
        Result<u32*, AllocError> result = try_allocate(integer);
    
        EXPECT_TRUE(result.is_ok());
        EXPECT_NE(result.unwrap(), nullptr);
        EXPECT_EQ(*result.unwrap(), integer);
        
        deallocate(result.unwrap());
    }

    TEST_P(memory_fixture, try_reallocate) {
        u32 bytes = GetParam();
        
        void* ptr = allocate_size(bytes);
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, 0);
        
        mem_set(ptr, 0xAB, bytes);
        
        void* ptr_save = allocate_size(bytes);
        mem_copy(ptr_save, ptr, bytes);
            
        EXPECT_TRUE(r_new_ptr.is_ok());
        EXPECT_NE(r_new_ptr.unwrap(), nullptr);
            
        deallocate(r_new_ptr.unwrap());
        deallocate(ptr_save);
    }

    TEST(memory, try_reallocate_too_big) {
        constexpr u32 bytes = 10;
        void* ptr = allocate_size(bytes);
        
        mem_set(ptr, 0xAB, bytes);
        
        void* ptr_save = allocate_size(bytes);
        mem_copy(ptr_save, ptr, bytes);
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, std::numeric_limits<u64>::max());
        
        EXPECT_TRUE(r_new_ptr.is_error());
    
        deallocate(ptr);
        deallocate(ptr_save);
    }

    TEST_P(memory_fixture, reallocate) {
        u64 bytes = GetParam();
        void* ptr = allocate_size(bytes);
        mem_set(ptr, 0xAB, bytes);
        
        void* ptr_save = allocate_size(bytes);
        mem_copy(ptr_save, ptr, bytes);

        void* new_ptr = reallocate(ptr, 0);
        
        EXPECT_NE(new_ptr, nullptr);
    
        deallocate(new_ptr);
        deallocate(ptr_save);
    }

    TEST_P(memory_fixture, reallocate_too_big) {
        u64 bytes = GetParam();
        void* ptr = allocate_size(bytes);
        mem_set(ptr, 0xAB, bytes);

        EXPECT_THROW(reallocate(ptr, std::numeric_limits<u64>::max()), AllocError);
         
        deallocate(ptr);
    }

    TEST_P(memory_fixture, deallocate) {
        u64 bytes = GetParam();
        void* ptr = allocate_size(bytes);

        EXPECT_NO_THROW(deallocate(ptr));
    }

    TEST(memory, deallocate_null) {
        EXPECT_DEATH(deallocate(nullptr), ".");
    }

    INSTANTIATE_TEST_SUITE_P(
            mem_sizes,
            memory_fixture,
            ::testing::Values(0, 1, 100, 1024, 2048)
    );
}

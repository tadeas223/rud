#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_random.hpp"
#include "catch2/generators/catch_generators_adapters.hpp"

#include "rud/base/memory.hpp"
#include "helpers.hpp"
#include "rud/base/types.hpp"
#include <catch2/catch_tostring.hpp>
#include <limits>

using namespace rud;

TEST_CASE("trying to allocate an arbitrary number of bytes works correctly", "[try_allocate_size]") {
    SECTION("allocation of 0 bytes") {
        constexpr u64 bytes = 0;

        Result<void*, AllocError> result = try_allocate_size(bytes);

        REQUIRE(result.ok);
        REQUIRE(result.unwrap() != nullptr);
        
        deallocate(result.unwrap());
    }

    SECTION("allocation of 1 byte") {
        constexpr u64 bytes = 1;

        Result<void*, AllocError> result = try_allocate_size(bytes);

        REQUIRE(result.ok);
        REQUIRE(result.unwrap() != nullptr);
        
        deallocate(result.unwrap());
    }

    SECTION("allocation of an arbitrary number of bytes") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 2048)));

        Result<void*, AllocError> result = try_allocate_size(bytes);

        REQUIRE(result.ok);
        REQUIRE(result.unwrap() != nullptr);
        
        deallocate(result.unwrap());
    }

    SECTION("alocation of too much bytes to allocate (u64 max)") {
        constexpr u64 bytes = std::numeric_limits<u64>::max();

        Result<void*, AllocError> result = try_allocate_size(bytes);

        REQUIRE(!result.ok);
    }
}

SCENARIO("push and pop works on a list", "[list]") {
    GIVEN("an empty list if integers") {
        C_List<u32> list;

        WHEN("a value is pushed into the list") {
            list.push(10);

            THEN("the list length is equal to 1") {
                REQUIRE(list.len() == 1);
            }
        }
    }
}


TEST_CASE("allocating an arbitrary number of bytes works correctly", "[allocate_size]") {
    SECTION("allocation of 0 bytes") {
        constexpr u64 bytes = 0;

        bool success = true;
        void* result = nullptr;
        try {
            result = allocate_size(bytes);
        } catch(AllocError& e) {
            success = false;
        }

        REQUIRE(success);
        REQUIRE(result != nullptr);
            
        deallocate(result);
    }

    SECTION("allocating 1 byte") {
        constexpr u64 bytes = 1;

        bool success = true;
        void* result = nullptr;
        try {
            result = allocate_size(bytes);
        } catch(AllocError& e) {
            success = false;
        }

        REQUIRE(success);
        REQUIRE(result != nullptr);
            
        deallocate(result);
    }

    SECTION("allocation of an arbitrary number of bytes") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 2048)));

        bool success = true;
        void* result = nullptr;
        try {
            result = allocate_size(bytes);
        } catch(AllocError& e) {
            success = false;
        }

        REQUIRE(success);
        REQUIRE(result != nullptr);
        
        deallocate(result);
    }

    SECTION("allocation of too much bytes to allocate (u64 max)") {
        constexpr u64 bytes = std::numeric_limits<u64>::max();

        bool success = true;
        try {
            allocate_size(bytes);
        } catch(AllocError& e) {
            success = false;
        }

        REQUIRE(!success);
    }
}

TEST_CASE("allocating a structure or a type works correctly", "[try_allocate]") {
    SECTION("allocation of an integer") {
        constexpr u32 integer = 10;

        Result<u32*, AllocError> result = try_allocate(integer);

        REQUIRE(result.ok);
        REQUIRE(result.unwrap() != nullptr);
        REQUIRE(*result.unwrap() == integer);
        
        deallocate(result.unwrap());
    }

    SECTION("allocation of an empty structure foo") {
        struct Foo {} foo;

        Result<Foo*, AllocError> result = try_allocate(foo);

        REQUIRE(result.ok);
        REQUIRE(result.unwrap() != nullptr);
            
        deallocate(result.unwrap());
    }
}

TEST_CASE("allocation of a structure or a type works correctly", "[allocate]") {
    SECTION("allocation of an integer") {
        constexpr u32 integer = 10;

        u32* result = allocate(integer);

        REQUIRE(result != nullptr);
        REQUIRE(*result == integer);
        
        deallocate(result);
    }

    SECTION("allocation an empty structure foo") {
        struct Foo {} foo;

        Foo* result = allocate(foo);

        REQUIRE(result != nullptr);
        deallocate(result);
    }
}

TEST_CASE("trying to reallocate an arbitrary number of bytes works correctly", "[try_reallocate]") {
    u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
    void* ptr = allocate_size(bytes);
    
    mem_set(ptr, 0xAB, bytes);
    
    void* ptr_save = allocate_size(bytes);
    mem_copy(ptr_save, ptr, bytes);

    SECTION("reallocation to 0 bytes") {
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, 0);
            
        REQUIRE(r_new_ptr.ok);
        REQUIRE(r_new_ptr.unwrap() != nullptr);
            
        deallocate(r_new_ptr.unwrap());
        deallocate(ptr_save);
    }

    SECTION("reallocation to 1 byte") {
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, 1);
        
        REQUIRE(r_new_ptr.ok); 
        REQUIRE(r_new_ptr.unwrap() != nullptr);
        REQUIRE(mem_equals(ptr_save, r_new_ptr.unwrap(), 1));
        
        deallocate(r_new_ptr.unwrap());
        deallocate(ptr_save);
    }

    SECTION("reallocation to a larger arbitrary size") {
        u64 new_size = GENERATE(Catch::Generators::take(50, Catch::Generators::random(1024, 2048)));
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, new_size);
        
        REQUIRE(r_new_ptr.ok);
        REQUIRE(r_new_ptr.unwrap() != nullptr);
        REQUIRE(mem_equals(ptr_save, r_new_ptr.unwrap(), bytes));

        deallocate(r_new_ptr.unwrap());
        deallocate(ptr_save);
    }

    SECTION("reallocation to an extremely large size") {
        Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, std::numeric_limits<u64>::max());
       
        REQUIRE(!r_new_ptr.ok);

        deallocate(ptr);
        deallocate(ptr_save);
    }
}

TEST_CASE("reallocation of an arbitrary number of bytes works correctly", "[reallocate]") {
    u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
    void* ptr = allocate_size(bytes);
    mem_set(ptr, 0xAB, bytes);
    
    void* ptr_save = allocate_size(bytes);
    mem_copy(ptr_save, ptr, bytes);

    SECTION("reallocation to 0 bytes") {
        void* new_ptr = reallocate(ptr, 0);
        
        REQUIRE(new_ptr != nullptr);

        deallocate(new_ptr);
        deallocate(ptr_save);
    }

    SECTION("reallocation to 1 byte") {
        void* new_ptr = reallocate(ptr, 1);

        REQUIRE(new_ptr != nullptr);
        REQUIRE(mem_equals(ptr, new_ptr, 1));
        
        deallocate(new_ptr);
        deallocate(ptr_save);
    }

    SECTION("reallocation to a larger arbitrary size") {
        u64 new_size = GENERATE(Catch::Generators::take(50, Catch::Generators::random(1024, 2048)));
        void* new_ptr = reallocate(ptr, new_size);
        
        REQUIRE(new_ptr != nullptr);
        REQUIRE(mem_equals(ptr_save, new_ptr, bytes));

        deallocate(new_ptr);
        deallocate(ptr_save);
    }

    SECTION("reallocation to an extremely large size") {
        REQUIRE_THROWS_AS(reallocate(ptr, std::numeric_limits<u64>::max()), AllocError);
        
        deallocate(ptr);
        deallocate(ptr_save);
    }

}

TEST_CASE("deallocate frees memory safely", "[deallocate]") {
    u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
    void* ptr = allocate_size(bytes);

    SECTION("deallocation") {
        REQUIRE_NOTHROW(deallocate(ptr));
    }
}

TEST_CASE("deallocate panics when deallocating nullptr", "[deallocate]") {
    SECTION("deallocation of a null pointer") {
        REQUIRE(code_aborts([] () {deallocate(nullptr);}));
    }
}


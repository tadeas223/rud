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

SCENARIO("trying to allocate an arbitrary number of bytes works correctly", "[try_allocate_size]") {
    GIVEN("0 bytes") {
        constexpr u64 bytes = 0;

        WHEN("allocating them") {
            Result<void*, AllocError> result = try_allocate_size(bytes);

            THEN("allocation is successful") {
                REQUIRE(result.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result.unwrap() != nullptr);
            }

            deallocate(result.unwrap());
        }
    }

    GIVEN("1 byte") {
        constexpr u64 bytes = 1;

        WHEN("allocating it") {
            Result<void*, AllocError> result = try_allocate_size(bytes);

            THEN("allocation is successful") {
                REQUIRE(result.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result.unwrap() != nullptr);
            }
            
            deallocate(result.unwrap());
        }
    }

    GIVEN("an arbitrary number of bytes") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 2048)));


        WHEN("allocating it") {
            Result<void*, AllocError> result = try_allocate_size(bytes);

            THEN("allocation is successful") {
                REQUIRE(result.ok);
            }
            
            THEN("the returned pointer is not null") {
                REQUIRE(result.unwrap() != nullptr);
            }
            
            deallocate(result.unwrap());
        }
    }

    GIVEN("too much bytes to allocate (u64 max)") {
        constexpr u64 bytes = std::numeric_limits<u64>::max();

        WHEN("allocating it") {
            Result<void*, AllocError> result = try_allocate_size(bytes);

            THEN("allocation is not successful") {
                REQUIRE(!result.ok);
            }
        }
    }
}

SCENARIO("allocating an arbitrary number of bytes works correctly", "[allocate_size]") {
    GIVEN("0 bytes") {
        constexpr u64 bytes = 0;

        WHEN("allocating them") {
            bool success = true;
            void* result = nullptr;
            try {
                result = allocate_size(bytes);
            } catch(AllocError& e) {
                success = false;
            }

            THEN("allocation is successful") {
                REQUIRE(success);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result != nullptr);
            }

            deallocate(result);
        }
    }

    GIVEN("1 byte") {
        constexpr u64 bytes = 1;

        WHEN("allocating it") {
            bool success = true;
            void* result = nullptr;
            try {
                result = allocate_size(bytes);
            } catch(AllocError& e) {
                success = false;
            }

            THEN("allocation is successful") {
                REQUIRE(success);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result != nullptr);
            }
            
            deallocate(result);
        }
    }

    GIVEN("an arbitrary number of bytes") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 2048)));


        WHEN("allocating it") {
            bool success = true;
            void* result = nullptr;
            try {
                result = allocate_size(bytes);
            } catch(AllocError& e) {
                success = false;
            }

            THEN("allocation is successful") {
                REQUIRE(success);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result != nullptr);
            }
            
            deallocate(result);
        }
    }

    GIVEN("too much bytes to allocate (u64 max)") {
        constexpr u64 bytes = std::numeric_limits<u64>::max();

        WHEN("allocating it") {
            bool success = true;
            try {
                allocate_size(bytes);
            } catch(AllocError& e) {
                success = false;
            }

            THEN("allocation is not successful") {
                REQUIRE(!success);
            }
        }
    }
}

SCENARIO("allocating a structure or a type works correctly", "[try_allocate]") {
    GIVEN("an integer") {
        constexpr u32 integer = 10;

        WHEN("allocating space for it and copying it into the memory") {
            Result<u32*, AllocError> result = try_allocate(integer);

            THEN("allocation is successful") {
                REQUIRE(result.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result.unwrap() != nullptr);
            }

            THEN("the memory at the pointer holds the same value as the integer") {
                REQUIRE(*result.unwrap() == integer);
            }
            
            deallocate(result.unwrap());
        }
    }

    GIVEN("an empty structure foo") {
        struct Foo {} foo;

        WHEN("allocating space for it and copying it into the memory") {
            Result<Foo*, AllocError> result = try_allocate(foo);

            THEN("allocation is successful") {
                REQUIRE(result.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(result.unwrap() != nullptr);
            }
            
            deallocate(result.unwrap());
        }
    }
}

SCENARIO("allocation of a structure or a type works correctly", "[allocate]") {
    GIVEN("an integer") {
        constexpr u32 integer = 10;

        WHEN("allocating space for it and copying it into the memory") {
            u32* result = allocate(integer);


            THEN("the returned pointer is not null") {
                REQUIRE(result != nullptr);
            }

            THEN("the memory at the pointer holds the same value as the integer") {
                REQUIRE(*result == integer);
            }
            
            deallocate(result);
        }
    }

    GIVEN("an empty structure foo") {
        struct Foo {} foo;

        WHEN("allocating space for it and copying it into the memory") {
            Foo* result = allocate(foo);

            THEN("the returned pointer is not null") {
                REQUIRE(result != nullptr);
            }
            
            deallocate(result);
        }
    }
}

SCENARIO("trying to reallocate an arbitrary number of bytes works correctly", "[try_reallocate]") {
    GIVEN("a previously allocated memory block with some data") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
        void* ptr = allocate_size(bytes);
        mem_set(ptr, 0xAB, bytes);
        
        void* ptr_save = allocate_size(bytes);
        mem_copy(ptr_save, ptr, bytes);

        WHEN("reallocating it to 0 bytes") {
            Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, 0);
            
            THEN("the allocation is successful") {
                REQUIRE(r_new_ptr.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(r_new_ptr.unwrap() != nullptr);
            }

            deallocate(r_new_ptr.unwrap());
        }

        WHEN("reallocating it to 1 byte") {
            Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, 1);
            
            THEN("the alloction is successful") {
                REQUIRE(r_new_ptr.ok); 
            }

            THEN("the returned pointer is not null") {
                REQUIRE(r_new_ptr.unwrap() != nullptr);
            }

            THEN("the contents are preserved") {
                REQUIRE(mem_equals(ptr_save, r_new_ptr.unwrap(), 1));
            }
            
            deallocate(r_new_ptr.unwrap());
        }

        WHEN("reallocating it to a larger arbitrary size") {
            u64 new_size = GENERATE(Catch::Generators::take(50, Catch::Generators::random(1024, 2048)));
            Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, new_size);
            
            THEN("the allocation is successful") {
                REQUIRE(r_new_ptr.ok);
            }

            THEN("the returned pointer is not null") {
                REQUIRE(r_new_ptr.unwrap() != nullptr);
            }

            THEN("the contents are preserved") {
                REQUIRE(mem_equals(ptr_save, r_new_ptr.unwrap(), bytes));
            }

            deallocate(r_new_ptr.unwrap());
        }

        WHEN("reallocating it to an extremely large size") {
            Result<void*, AllocError> r_new_ptr = try_reallocate(ptr, std::numeric_limits<u64>::max());
            
            THEN("the allocation is not successful") {
                REQUIRE(!r_new_ptr.ok);
            }

            deallocate(ptr);
        }

        deallocate(ptr_save);
    }
}

SCENARIO("reallocation of an arbitrary number of bytes works correctly", "[reallocate]") {
    GIVEN("a previously allocated memory block with some data") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
        void* ptr = allocate_size(bytes);
        mem_set(ptr, 0xAB, bytes);
        
        void* ptr_save = allocate_size(bytes);
        mem_copy(ptr_save, ptr, bytes);

        WHEN("reallocating it to 0 bytes") {
            void* new_ptr = reallocate(ptr, 0);
            
            THEN("the returned pointer is not null") {
                REQUIRE(new_ptr != nullptr);
            }

            deallocate(new_ptr);
        }

        WHEN("reallocating it to 1 byte") {
            void* new_ptr = reallocate(ptr, 1);

            THEN("the returned pointer is not null") {
                REQUIRE(new_ptr != nullptr);
            }

            THEN("the contents are preserved") {
                REQUIRE(mem_equals(ptr, new_ptr, 1));
            }
            
            deallocate(new_ptr);
        }

        WHEN("reallocating it to a larger arbitrary size") {
            u64 new_size = GENERATE(Catch::Generators::take(50, Catch::Generators::random(1024, 2048)));
            void* new_ptr = reallocate(ptr, new_size);
            
            THEN("the returned pointer is not null") {
                REQUIRE(new_ptr != nullptr);
            }

            THEN("the contents are preserved") {
                REQUIRE(mem_equals(ptr_save, new_ptr, bytes));
            }

            deallocate(new_ptr);
        }

        WHEN("reallocating it to an extremely large size") {
            REQUIRE_THROWS_AS(reallocate(ptr, std::numeric_limits<u64>::max()), AllocError);
            
            deallocate(ptr);
        }

        deallocate(ptr_save);
    }
}

SCENARIO("deallocate frees memory safely", "[deallocate]") {
    GIVEN("a previously allocated block") {
        u64 bytes = GENERATE(Catch::Generators::take(100, Catch::Generators::random(1, 1024)));
        void* ptr = allocate_size(bytes);

        WHEN("deallocating it") {
            THEN("it does not throw") {
                REQUIRE_NOTHROW(deallocate(ptr));
            }
        }
    }
}

SCENARIO("deallocate panics when deallocating nullptr", "[deallocate]") {
    GIVEN("a null pointer") {
        WHEN("deallocating it") {
            THEN("it panics") {
                REQUIRE(code_aborts([] () {deallocate(nullptr);}));
            }
        }
    }
}


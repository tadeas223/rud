#include <gtest/gtest.h>
#include "rud/base/option.hpp"
#include "rud/base/string.hpp"

using namespace rud;

// A trivial struct for testing Option with structs
struct Foo {
    u32 a;
    u32 b;
};

//
// Construction and basic usage
//
TEST(Option, MakeSome) {
    auto opt = Option<u32>::make_some(42);
    EXPECT_TRUE(opt.some);
    EXPECT_EQ(opt.value, 42);
}

TEST(Option, MakeNone) {
    auto opt = Option<u32>::make_none();
    EXPECT_FALSE(opt.some);
}

//
// unwrap() behavior
//
TEST(Option, UnwrapSome) {
    auto opt = Option<u32>::make_some(99);
    EXPECT_EQ(opt.unwrap(), 99);
}

TEST(Option, UnwrapNoneTriggersDeath) {
    auto opt = Option<u32>::make_none();
    EXPECT_DEATH(opt.unwrap(), ".");
}

//
// or_panic() behavior
//
TEST(Option, OrPanicSome) {
    auto opt = Option<u32>::make_some(7);
    EXPECT_EQ(opt.or_panic(), 7);
}

TEST(Option, OrPanicNoneTriggersDeath) {
    auto opt = Option<u32>::make_none();
    EXPECT_DEATH(opt.or_panic(), ".");
}

//
// Works with non-trivial types
//
TEST(Option, WorksWithStructs) {
    Foo foo{1, 2};
    auto opt = Option<Foo>::make_some(foo);
    EXPECT_TRUE(opt.some);
    EXPECT_EQ(opt.value.a, 1);
    EXPECT_EQ(opt.value.b, 2);
}

//
// Works with C_StringAlloc
//
TEST(Option, WorksWithCStringAlloc) {
    auto str = C_StringAlloc::make_copy("hello");
    auto opt = Option<C_StringAlloc>::make_some(str);

    EXPECT_TRUE(opt.some);
    EXPECT_EQ(opt.value.len, 5);

    // Clean up memory
    opt.value.destroy();
}

//
// Edge cases
//
TEST(Option, OptionOfZero) {
    auto opt = Option<u32>::make_some(0);
    EXPECT_TRUE(opt.some);
    EXPECT_EQ(opt.value, 0);
}

TEST(Option, OptionOfEmptyStruct) {
    struct Empty {};
    auto opt = Option<Empty>::make_some(Empty{});
    EXPECT_TRUE(opt.some);
}

//
// Repeated creation and destruction
//
TEST(Option, MultipleMakeSomeAndDestroy) {
    for (u32 i = 0; i < 100; ++i) {
        auto str = C_StringAlloc::make_copy("test");
        auto opt = Option<C_StringAlloc>::make_some(str);
        EXPECT_TRUE(opt.some);
        opt.value.destroy();
    }
}


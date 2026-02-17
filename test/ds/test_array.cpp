#include <gtest/gtest.h>
#include "rud/ds/array.hpp"

using namespace rud;
using namespace rud::ds;

//
// construction and  basic uses
//

TEST(Array, Construction) {
    auto array = Array<u32, 100>::make();

    EXPECT_EQ(array.len(), 100);
}

//
// set_get
//

TEST(Array, SetGet) {
    auto array = Array<u32, 100>::make();

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, i + 1);

    for (u32 i = 0; i < array.len(); ++i) {
        EXPECT_EQ(*array.get(i), i + 1);
        EXPECT_EQ(*array[i], i + 1);
    }
}

TEST(Array, OverwriteValues) {
    auto array = Array<u32, 32>::make();

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, i);

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, i + 100);

    for (u32 i = 0; i < array.len(); ++i)
        EXPECT_EQ(*array[i], i + 100);
}

//
// boundary checks
//

TEST(Array, GetOutOfBoundsAsserts) {
    auto array = Array<u32, 10>::make();
    EXPECT_DEATH(array.get(10), ".");
}

TEST(Array, SetOutOfBoundsAsserts) {
    auto array = Array<u32, 10>::make();
    EXPECT_DEATH(array.set(10, 1), ".");
}

//
// data() correctness
//

TEST(Array, DataPointerMatchesStorage) {
    auto array = Array<u32, 64>::make();

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, i * 3);

    const u32* data = array.data();

    for (u32 i = 0; i < array.len(); ++i)
        EXPECT_EQ(data[i], *array[i]);
}

TEST(Array, DataPointerIsStable) {
    auto array = Array<u32, 16>::make();

    const u32* ptr = array.data();

    array.set(5, 42);
    EXPECT_EQ(ptr[5], 42);
}

//
// destroy_contents behavior
//

static u32 counter = 0;
TEST(Array, DestroyContentsVisitsAllElements) {
    auto array = Array<u32, 50>::make();


    array.destroy_contents([](u32) {
        counter++;
    });

    EXPECT_EQ(counter, 50);
}

//
// LinearView integration
//

TEST(Array, LinearViewMatchesArray) {
    auto array = Array<u32, 100>::make();

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, i + 7);

    auto view = array.to_linear_view();

    EXPECT_EQ(view.len(), array.len());

    for (u32 i = 0; i < view.len(); ++i) {
        EXPECT_EQ(*view[i], *array[i]);
    }
}

//
// non-trivial type safety
//

struct Foo {
    u32 value;
};

TEST(Array, WorksWithStructTypes) {
    auto array = Array<Foo, 8>::make();

    for (u32 i = 0; i < array.len(); ++i)
        array.set(i, Foo{i});

    for (u32 i = 0; i < array.len(); ++i)
        EXPECT_EQ(array[i]->value, i);
}


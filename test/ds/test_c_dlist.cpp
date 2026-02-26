#include <gtest/gtest.h>
#include "rud/ds/c_dlist.hpp"

using namespace rud;
using namespace rud::ds;

//
// Construction
//

TEST(C_DList, MakeInitialState) {
    auto list = C_DList<u32>::make();

    EXPECT_EQ(list.len(), 0);
    EXPECT_EQ(list.head(), nullptr);
    EXPECT_EQ(list.tail(), nullptr);

    list.destroy();
}

//
// Push (back)
//

TEST(C_DList, PushIncreasesLengthAndMaintainsOrder) {
    auto list = C_DList<u32>::make();

    const u32 N = 200;

    for (u32 i = 0; i < N; ++i) {
        list.push(i);
        EXPECT_EQ(list.len(), i + 1);
        EXPECT_EQ(*list.peek(), i);
    }

    for (u32 i = 0; i < N; ++i)
        EXPECT_EQ(*list[i], i);

    list.destroy();
}

//
// Push Front
//

TEST(C_DList, PushFrontMaintainsReverseOrder) {
    auto list = C_DList<u32>::make();

    const u32 N = 150;

    for (u32 i = 0; i < N; ++i)
        list.push_front(i);

    EXPECT_EQ(list.len(), N);

    for (u32 i = 0; i < N; ++i)
        EXPECT_EQ(*list[i], N - 1 - i);

    EXPECT_EQ(*list.peek_front(), N - 1);
    EXPECT_EQ(*list.peek(), 0);

    list.destroy();
}

//
// Pop (back)
//

TEST(C_DList, PopReturnsLastElementAndDecreasesLength) {
    auto list = C_DList<u32>::make();

    const u32 N = 120;

    for (u32 i = 0; i < N; ++i)
        list.push(i);

    for (u32 i = N; i > 0; --i) {
        EXPECT_EQ(list.pop(), i - 1);
        EXPECT_EQ(list.len(), i - 1);
    }

    EXPECT_EQ(list.len(), 0);
    EXPECT_EQ(list.head(), nullptr);
    EXPECT_EQ(list.tail(), nullptr);

    list.destroy();
}

//
// Pop Front
//

TEST(C_DList, PopFrontReturnsFirstElement) {
    auto list = C_DList<u32>::make();

    const u32 N = 100;

    for (u32 i = 0; i < N; ++i)
        list.push(i);

    for (u32 i = 0; i < N; ++i) {
        EXPECT_EQ(list.pop_front(), i);
        EXPECT_EQ(list.len(), N - i - 1);
    }

    EXPECT_EQ(list.len(), 0);

    list.destroy();
}

//
// Remove by index
//

TEST(C_DList, RemoveFromMiddleMaintainsOrder) {
    auto list = C_DList<u32>::make();

    const u32 N = 50;

    for (u32 i = 0; i < N; ++i)
        list.push(i);

    u32 removed = list.remove(25);
    EXPECT_EQ(removed, 25);
    EXPECT_EQ(list.len(), N - 1);

    for (u32 i = 0; i < list.len(); ++i) {
        if (i < 25)
            EXPECT_EQ(*list[i], i);
        else
            EXPECT_EQ(*list[i], i + 1);
    }

    list.destroy();
}

//
// Set
//

TEST(C_DList, SetOverwritesValue) {
    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 40; ++i)
        list.push(i);

    for (u32 i = 0; i < 40; ++i)
        list.set(i, i + 100);

    for (u32 i = 0; i < 40; ++i)
        EXPECT_EQ(*list[i], i + 100);

    list.destroy();
}

//
// Get / Operator[]
//

TEST(C_DList, GetAndOperatorReturnSamePointer) {
    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 60; ++i)
        list.push(i * 3);

    for (u32 i = 0; i < list.len(); ++i) {
        EXPECT_EQ(list.get(i), list[i]);
        EXPECT_EQ(*list.get(i), i * 3);
    }

    list.destroy();
}

//
// Boundary checks
//

TEST(C_DList, GetOutOfBoundsAsserts) {
    auto list = C_DList<u32>::make();
    list.push(1);
    EXPECT_DEATH(list.get(1), ".");
    list.destroy();
}

TEST(C_DList, RemoveOutOfBoundsAsserts) {
    auto list = C_DList<u32>::make();
    list.push(1);
    EXPECT_DEATH(list.remove(1), ".");
    list.destroy();
}

TEST(C_DList, PopEmptyAsserts) {
    auto list = C_DList<u32>::make();
    EXPECT_DEATH(list.pop(), ".");
    list.destroy();
}

TEST(C_DList, PopFrontEmptyAsserts) {
    auto list = C_DList<u32>::make();
    EXPECT_DEATH(list.pop_front(), ".");
    list.destroy();
}

//
// Clear
//

TEST(C_DList, ClearResetsListState) {
    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 75; ++i)
        list.push(i);

    list.clear();

    EXPECT_EQ(list.len(), 0);
    EXPECT_EQ(list.head(), nullptr);
    EXPECT_EQ(list.tail(), nullptr);

    list.destroy();
}

//
// Clear with destroy callback
//

u32 destroy_counter = 0;

void destroy_func(u32) {
    destroy_counter++;
}

TEST(C_DList, ClearWithDestroyVisitsAllElements) {
    destroy_counter = 0;

    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 80; ++i)
        list.push(i);

    list.clear(destroy_func);

    EXPECT_EQ(destroy_counter, 80);
    EXPECT_EQ(list.len(), 0);

    list.destroy();
}

//
// Destroy with callback
//

TEST(C_DList, DestroyWithCallbackVisitsAllElements) {
    destroy_counter = 0;

    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 90; ++i)
        list.push(i);

    list.destroy(destroy_func);

    EXPECT_EQ(destroy_counter, 90);
}

//
// LinearView integration
//

TEST(C_DList, LinearViewMatchesList) {
    auto list = C_DList<u32>::make();

    for (u32 i = 0; i < 110; ++i)
        list.push(i + 5);

    auto view = list.to_linear_view();

    EXPECT_EQ(view.len(), list.len());

    for (u32 i = 0; i < view.len(); ++i)
        EXPECT_EQ(*view[i], *list[i]);

    list.destroy();
}

//
// Works with struct types
//

struct Foo {
    u32 value;
};

TEST(C_DList, WorksWithStructType) {
    auto list = C_DList<Foo>::make();

    for (u32 i = 0; i < 30; ++i)
        list.push(Foo{i});

    for (u32 i = 0; i < 30; ++i)
        EXPECT_EQ(list[i]->value, i);

    list.destroy();
}


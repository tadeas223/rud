#include <gtest/gtest.h>
#include "rud/ds/c_darray.hpp"

using namespace rud::ds;
using namespace rud;

//
// Construction
//

TEST(C_DArray, DefaultConstruction) {
    auto arr = C_DArray<u32>::make();
    EXPECT_EQ(arr.len(), 0);
    EXPECT_GE(arr.cap(), 0);
}

TEST(C_DArray, ConstructionWithCapacity) {
    for (u32 cap = 0; cap <= 100; cap += 10) {
        auto arr = C_DArray<u32>::make(cap);
        EXPECT_EQ(arr.len(), 0);
        EXPECT_GE(arr.cap(), cap);
        arr.destroy();
    }
}

//
// Push / Pop
//

TEST(C_DArray, PushAndPopBack) {
    auto arr = C_DArray<u32>::make(5);
    for (u32 i = 0; i < 10; ++i) {
        arr.push(i * 3);
        EXPECT_EQ(*arr.peek(), i * 3);
    }

    for (u32 i = 9; i < 10; --i) {
        u32 val = arr.pop();
        EXPECT_EQ(val, i * 3);
    }

    EXPECT_EQ(arr.len(), 0);
    arr.destroy();
}

TEST(C_DArray, PushAndPopFront) {
    auto arr = C_DArray<u32>::make(5);
    for (u32 i = 0; i < 10; ++i) {
        arr.push_front(i * 2);
        EXPECT_EQ(*arr.peek_front(), i * 2);
    }

    for (u32 i = 9; i < 10; --i) {
        u32 val = arr.pop_front();
        EXPECT_EQ(val, i * 2);
    }

    EXPECT_EQ(arr.len(), 0);
    arr.destroy();
}

//
// Set / Get / Remove
//

TEST(C_DArray, SetAndGet) {
    auto arr = C_DArray<u32>::make(20);
    for (u32 i = 0; i < 20; ++i) arr.push(0);

    for (u32 i = 0; i < 20; ++i) arr.set(i, i + 1);

    for (u32 i = 0; i < 20; ++i) {
        EXPECT_EQ(*arr.get(i), i + 1);
        EXPECT_EQ(*arr[i], i + 1);
    }
    arr.destroy();
}

TEST(C_DArray, RemoveValues) {
    auto arr = C_DArray<u32>::make();
    for (u32 i = 0; i < 10; ++i) arr.push(i);

    for (u32 i = 0; i < 5; ++i) {
        u32 val = arr.remove(0);
        EXPECT_EQ(val, i);
    }

    EXPECT_EQ(arr.len(), 5);
    for (u32 i = 0; i < 5; ++i) EXPECT_EQ(*arr[i], i + 5);
    arr.destroy();
}

//
// Clear
//

TEST(C_DArray, ClearWithoutFunc) {
    auto arr = C_DArray<u32>::make();
    for (u32 i = 0; i < 10; ++i) arr.push(i);

    arr.clear();
    EXPECT_EQ(arr.len(), 0);
    arr.destroy();
}

static u32 counter = 0;
TEST(C_DArray, ClearWithFunc) {
    auto arr = C_DArray<u32>::make();
    for (u32 i = 0; i < 10; ++i) arr.push(i);

    arr.clear([](u32 val) { counter += val; });
    EXPECT_EQ(counter, 45); // sum of 0..9
    EXPECT_EQ(arr.len(), 0);
    arr.destroy();
}

//
// Peek
//

TEST(C_DArray, PeekBackAndFront) {
    auto arr = C_DArray<u32>::make();
    arr.push(42);
    arr.push(99);

    EXPECT_EQ(*arr.peek(), 99);
    EXPECT_EQ(*arr.peek_front(), 42);
    arr.destroy();
}

//
// Resize
//

TEST(C_DArray, ResizeIncreasesCapacity) {
    auto arr = C_DArray<u32>::make(2);
    arr.push(1);
    arr.push(2);

    u32 old_cap = arr.cap();
    arr.resize(old_cap + 10);

    EXPECT_GE(arr.cap(), old_cap + 10);
    EXPECT_EQ(arr.len(), 2);
    arr.destroy();
}

TEST(C_DArray, ResizePreservesData) {
    auto arr = C_DArray<u32>::make(3);
    arr.push(10);
    arr.push(20);
    arr.push(30);

    arr.resize(10);

    EXPECT_EQ(arr.len(), 3);
    EXPECT_EQ(*arr[0], 10);
    EXPECT_EQ(*arr[1], 20);
    EXPECT_EQ(*arr[2], 30);
    arr.destroy();
}

//
// LinearView
//

TEST(C_DArray, LinearViewMatchesData) {
    auto arr = C_DArray<u32>::make();
    for (u32 i = 0; i < 20; ++i) arr.push(i * 2);

    auto view = arr.to_linear_view();
    EXPECT_EQ(view.len(), arr.len());

    for (u32 i = 0; i < view.len(); ++i)
        EXPECT_EQ(*view[i], *arr[i]);

    arr.destroy();
}

//
// Destroy to pointer
//

TEST(C_DArray, DestroyToPointer) {
    auto arr = C_DArray<u32>::make();
    for (u32 i = 0; i < 5; ++i) arr.push(i);

    u32* ptr = arr.destroy_to_pointer();
    for (u32 i = 0; i < 5; ++i) EXPECT_EQ(ptr[i], i);

    delete[] ptr;
}

//
// Works with non-trivial types
//

struct Foo {
    u32 value;
};

TEST(C_DArray, WorksWithStructTypes) {
    auto arr = C_DArray<Foo>::make();
    for (u32 i = 0; i < 8; ++i) arr.push(Foo{i});

    for (u32 i = 0; i < 8; ++i) EXPECT_EQ(arr[i]->value, i);
    arr.destroy();
}


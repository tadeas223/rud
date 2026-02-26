#include <gtest/gtest.h>
#include "rud/ds/c_list.hpp"

using namespace rud;
using namespace rud::ds;

//
// construction and destruction
//

TEST(C_List, MakeEmpty) {
    auto list = C_List<u32>::make();

    EXPECT_EQ(list.len(), 0u);
    EXPECT_EQ(list.head(), nullptr);
    EXPECT_EQ(list.tail(), nullptr);

    list.destroy(); // valid even on empty
}

TEST(C_List, DestroyLeavesInvalid) {
    auto list = C_List<u32>::make();
    list.push(42);
    list.destroy();

    // After destroy, accessing members is undefined.
    // We cannot test them safely. Just ensure no crash on destroy itself.
}

//
// push and pop back/front
//

TEST(C_List, PushPopBack) {
    auto list = C_List<u32>::make();

    const u32 N = 50;
    for (u32 i = 0; i < N; ++i) {
        list.push(i);
        EXPECT_EQ(list.len(), i + 1);
        EXPECT_EQ(list.tail()->value, i);
    }

    for (u32 i = 0; i < N; ++i) {
        u32 val = list.pop();
        EXPECT_EQ(val, N - i - 1);
    }

    EXPECT_EQ(list.len(), 0u);
    list.destroy();
}

TEST(C_List, PushPopFront) {
    auto list = C_List<u32>::make();

    const u32 N = 50;
    for (u32 i = 0; i < N; ++i) {
        list.push_front(i);
        EXPECT_EQ(list.len(), i + 1);
        EXPECT_EQ(list.head()->value, i);
    }

    for (u32 i = 0; i < N; ++i) {
        u32 val = list.pop_front();
        EXPECT_EQ(val, N - i - 1);
    }

    EXPECT_EQ(list.len(), 0u);
    list.destroy();
}

//
// random access and set
//

TEST(C_List, GetAndSet) {
    auto list = C_List<u32>::make();

    const u32 N = 20;
    for (u32 i = 0; i < N; ++i)
        list.push(i * 2);

    for (u32 i = 0; i < N; ++i)
        EXPECT_EQ(*list[i], i * 2);

    for (u32 i = 0; i < N; ++i)
        list.set(i, i * 3);

    for (u32 i = 0; i < N; ++i)
        EXPECT_EQ(*list.get(i), i * 3);

    list.destroy();
}

TEST(C_List, GetOutOfBounds) {
    auto list = C_List<u32>::make();
    list.push(1);
    EXPECT_DEATH(list.get(1), ".");
    EXPECT_DEATH(list[1], ".");
    list.destroy();
}

TEST(C_List, SetOutOfBounds) {
    auto list = C_List<u32>::make();
    list.push(1);
    EXPECT_DEATH(list.set(1, 42), ".");
    list.destroy();
}

//
// remove and clear
//

TEST(C_List, RemoveElements) {
    auto list = C_List<u32>::make();

    const u32 N = 10;
    for (u32 i = 0; i < N; ++i)
        list.push(i);

    // remove first
    EXPECT_EQ(list.remove(0), 0u);
    EXPECT_EQ(list.len(), N - 1);

    // remove last
    EXPECT_EQ(list.remove(list.len() - 1), N - 1);
    EXPECT_EQ(list.len(), N - 2);

    // remove middle
    u32 middle_val = list.remove(3);
    EXPECT_EQ(middle_val, 4u);
    EXPECT_EQ(list.len(), N - 3);

    list.destroy();
}

TEST(C_List, ClearEmptiesList) {
    auto list = C_List<u32>::make();

    for (u32 i = 0; i < 10; ++i)
        list.push(i);

    list.clear();
    EXPECT_EQ(list.len(), 0u);
    EXPECT_EQ(list.head(), nullptr);
    EXPECT_EQ(list.tail(), nullptr);

    list.destroy();
}

//
// peek operations
//

TEST(C_List, PeekFrontAndBack) {
    auto list = C_List<u32>::make();

    list.push(1);
    list.push(2);
    list.push_front(0);

    EXPECT_EQ(*list.peek_front(), 0u);
    EXPECT_EQ(*list.peek(), 2u);

    list.destroy();
}

//
// LinearView integration
//

TEST(C_List, LinearViewMatchesList) {
    auto list = C_List<u32>::make();

    const u32 N = 30;
    for (u32 i = 0; i < N; ++i)
        list.push(i + 7);

    auto view = list.to_linear_view();
    EXPECT_EQ(view.len(), list.len());

    for (u32 i = 0; i < view.len(); ++i)
        EXPECT_EQ(*view[i], *list[i]);

    list.destroy();
}

//
// non-trivial struct type
//

struct Foo {
    u32 a;
    u32 b;
};

TEST(C_List, WorksWithStructs) {
    auto list = C_List<Foo>::make();

    const u32 N = 5;
    for (u32 i = 0; i < N; ++i)
        list.push(Foo{i, i * 2});

    for (u32 i = 0; i < N; ++i) {
        EXPECT_EQ(list[i]->a, i);
        EXPECT_EQ(list[i]->b, i * 2);
    }

    list.destroy();
}


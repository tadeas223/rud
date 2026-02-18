#include <gtest/gtest.h>
#include "rud/base/result.hpp"
#include "rud/base/string.hpp"

using namespace rud;

//
// Basic Result<V, E> construction
//

TEST(Result, MakeOkAndError_Int) {
    for (int i = -10; i <= 10; ++i) {
        auto r_ok = Result<int, int>::make_ok(i);
        EXPECT_TRUE(r_ok.ok);
        EXPECT_EQ(r_ok.unwrap(), i);

        auto r_err = Result<int, int>::make_error(i);
        EXPECT_FALSE(r_err.ok);
        EXPECT_EQ(r_err.unwrap_error(), i);
    }
}

//
// Result<void, E> construction
//

TEST(Result, MakeOkAndError_Void) {
    auto r_ok = Result<void, int>::make_ok();
    EXPECT_TRUE(r_ok.ok);

    for (int i = -5; i <= 5; ++i) {
        auto r_err = Result<void, int>::make_error(i);
        EXPECT_FALSE(r_err.ok);
        EXPECT_EQ(r_err.unwrap_error(), i);
    }
}

//
// try_or with primitive type
//

TEST(Result, TryOr_Int) {
    for (int i = 0; i < 5; ++i) {
        auto r_ok = Result<int, int>::make_ok(i);
        int val = r_ok.try_or([]() { return 42; });
        EXPECT_EQ(val, i);

        auto r_err = Result<int, int>::make_error(i);
        val = r_err.try_or([&]() { return i + 100; });
        EXPECT_EQ(val, i + 100);
    }
}

//
// try_or with void
//

TEST(Result, TryOr_Void) {
    int counter = 0;
    auto r_ok = Result<void, int>::make_ok();
    r_ok.try_or([&]() { counter++; });
    EXPECT_EQ(counter, 0);

    auto r_err = Result<void, int>::make_error(1);
    r_err.try_or([&]() { counter++; });
    EXPECT_EQ(counter, 1);
}

//
// unwrap and unwrap_error panics
//

TEST(ResultDeathTest, UnwrapOrPanic_Int) {
    auto r_ok = Result<int, int>::make_ok(10);
    EXPECT_EQ(r_ok.unwrap(), 10);
    EXPECT_EQ(r_ok.or_panic(), 10);

    auto r_err = Result<int, int>::make_error(5);
    EXPECT_DEATH(r_err.unwrap(), ".");
    EXPECT_DEATH(r_err.or_panic(), ".");
}

TEST(ResultDeathTest, UnwrapOrPanic_Void) {
    auto r_ok = Result<void, int>::make_ok();
    EXPECT_NO_THROW(r_ok.unwrap());
    EXPECT_NO_THROW(r_ok.or_panic());

    auto r_err = Result<void, int>::make_error(1);
    EXPECT_DEATH(r_err.unwrap(), ".");
    EXPECT_DEATH(r_err.or_panic(), ".");
}

//
// Non-trivial type: C_StringAlloc
//

TEST(Result, ResultWithCStringAlloc) {
    const char* test_strs[] = {"", "a", "hello world"};

    // Test Result<C_StringAlloc, int>
    for (const char* s : test_strs) {
        auto str_copy = C_StringAlloc::make_copy_cstr(s);
        auto r_ok = Result<C_StringAlloc, int>::make_ok(str_copy);
        EXPECT_TRUE(r_ok.ok);

        auto unwrapped = r_ok.unwrap();
        EXPECT_TRUE(unwrapped.view() == StringView::make_cstr(s));

        unwrapped.destroy(); // destroy after test
    }

    // Test Result<int, C_StringAlloc>
    for (const char* s : test_strs) {
        auto str_copy = C_StringAlloc::make_copy_cstr(s);
        auto r_err = Result<int, C_StringAlloc>::make_error(str_copy);
        EXPECT_FALSE(r_err.ok);

        auto err_unwrapped = r_err.unwrap_error();
        EXPECT_TRUE(err_unwrapped.view() == StringView::make_cstr(s));

        err_unwrapped.destroy(); // destroy after test
    }
}

//
// Exception message checks (non-trivial integration)
//

TEST(ResultDeathTest, Except_PanicsOnError_Int) {
    auto r_err = Result<int, int>::make_error(42);
    EXPECT_DEATH(r_err.except(Lit("test")), ".");
}

TEST(ResultDeathTest, Except_PanicsOnError_Void) {
    auto r_err = Result<void, int>::make_error(42);
    EXPECT_DEATH(r_err.except(Lit("fail")), ".");
}


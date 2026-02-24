#include "gtest/gtest.h"
#include "rud/base/result.hpp"
#include "rud/base/string.hpp"

using namespace rud;

namespace {
    enum struct TestError {
        Failed
    };

    TEST(Result, make_ok) {
        auto result = Result<u32, TestError>::make_ok(10);

        EXPECT_TRUE(result.is_ok());
        EXPECT_FALSE(result.is_error());
        EXPECT_EQ(result.unwrap(), 10);
        
        EXPECT_DEATH(result.unwrap_error(), ".");
    }
    
    TEST(Result, make_error) {
        auto result = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_TRUE(result.is_error());
        EXPECT_FALSE(result.is_ok());
        EXPECT_EQ(result.unwrap_error(), TestError::Failed);
        
        EXPECT_DEATH(result.unwrap(), ".");
    }

    TEST(Result, or_except) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        EXPECT_EQ(result_ok.or_expect(Lit("message")), 10);

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_expect(Lit("message")), "message"); 
    }

    TEST(Result, or_panic) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        EXPECT_EQ(result_ok.or_panic(), 10);

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_panic(), "."); 
    }

    TEST(Result_void, make_ok) {
        auto result = Result<void, TestError>::make_ok();

        EXPECT_TRUE(result.is_ok());
        EXPECT_FALSE(result.is_error());
        result.unwrap();
        
        EXPECT_DEATH(result.unwrap_error(), ".");
    }
    
    TEST(Result_void, make_error) {
        auto result = Result<void, TestError>::make_error(TestError::Failed);

        EXPECT_TRUE(result.is_error());
        EXPECT_FALSE(result.is_ok());
        EXPECT_EQ(result.unwrap_error(), TestError::Failed);
        
        EXPECT_DEATH(result.unwrap(), ".");
    }

    TEST(Result_void, or_except) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        result_ok.or_expect(Lit("message"));

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_expect(Lit("message")), "message"); 
    }

    TEST(Result_void, or_panic) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        result_ok.or_panic();

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_panic(), "."); 
    }
}

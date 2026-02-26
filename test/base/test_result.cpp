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

    TEST(Result, or_default) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        EXPECT_EQ(result_ok.or_default(100), 10);

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_EQ(result_err.or_default(100), 100); 
    }

    TEST(Result, or_panic) {
        auto result_ok = Result<u32, TestError>::make_ok(10);

        EXPECT_EQ(result_ok.or_panic(), 10);

        auto result_err = Result<u32, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_panic(), "."); 
    }

    TEST(Result, destroy_contents) {
        Result result_ok = Result<u32, TestError>::make_ok(10);

        u32 counter = 0;
        result_ok.destroy_contents(Destroyer<u32>::make(&counter, [](void* ctx, u32 value){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));

        EXPECT_EQ(counter, 1);
        
        Result result_err = Result<u32, TestError>::make_error(TestError::Failed);

        counter = 0;
        result_err.destroy_contents(Destroyer<u32>::make(&counter, [](void* ctx, u32 value){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));
    
        EXPECT_EQ(counter, 0);
    }

    TEST(Result, destroy_error) {
        Result result_ok = Result<u32, TestError>::make_ok(10);

        u32 counter = 0;
        result_ok.destroy_error(Destroyer<TestError>::make(&counter, [](void* ctx, TestError error){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));

        EXPECT_EQ(counter, 0);
        
        Result result_err = Result<u32, TestError>::make_error(TestError::Failed);

        counter = 0;
        result_err.destroy_error(Destroyer<TestError>::make(&counter, [](void* ctx, TestError error){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));

        EXPECT_EQ(counter, 1);
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
        auto result_ok = Result<void, TestError>::make_ok();

        result_ok.or_expect(Lit("message"));

        auto result_err = Result<void, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_expect(Lit("message")), "message"); 
    }

    TEST(Result_void, or_panic) {
        auto result_ok = Result<void, TestError>::make_ok();

        result_ok.or_panic();

        auto result_err = Result<void, TestError>::make_error(TestError::Failed);

        EXPECT_DEATH(result_err.or_panic(), "."); 
    }

    TEST(Result_void, destroy_contents) {
        Result result_err = Result<u32, TestError>::make_error(TestError::Failed);

        u32 counter = 0;
        result_err.destroy_contents(Destroyer<u32>::make(&counter, [](void* ctx, u32 value){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));
    
        EXPECT_EQ(counter, 0);
    }

    TEST(Result_void, destroy_error) {
        Result result_ok = Result<u32, TestError>::make_ok(10);

        u32 counter = 0;
        result_ok.destroy_error(Destroyer<TestError>::make(&counter, [](void* ctx, TestError error){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));

        EXPECT_EQ(counter, 0);
        
        Result result_err = Result<u32, TestError>::make_error(TestError::Failed);

        counter = 0;
        result_err.destroy_error(Destroyer<TestError>::make(&counter, [](void* ctx, TestError error){
            u32* counter = reinterpret_cast<u32*>(ctx);
            *counter += 1;
        }));

        EXPECT_EQ(counter, 1);
    }
}

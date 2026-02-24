#include "gtest/gtest.h"

#include "rud/base/option.hpp"
#include "rud/base/string.hpp"

using namespace rud;

namespace {
    TEST(Option, make_some) {
        Option<u32> option = Option<u32>::make_some(10);
        
        EXPECT_TRUE(option.has_some());
        EXPECT_FALSE(option.has_none());
        
        EXPECT_EQ(option.unwrap(), 10);
    }
    
    TEST(Option, make_none) {
        Option<u32> option = Option<u32>::make_none();
        
        EXPECT_FALSE(option.has_some());
        EXPECT_TRUE(option.has_none());
    
        EXPECT_DEATH({ option.unwrap(); }, ".");
    }
    
    TEST(Option, or_panic) {
        Option<u32> option = Option<u32>::make_some(10);
        EXPECT_EQ(option.or_panic(), 10);
    
        Option<u32> bad_option = Option<u32>::make_none();
        EXPECT_DEATH(bad_option.or_panic(), ".");
    }
    
    TEST(Option, or_except) {
        Option<u32> option = Option<u32>::make_some(10);
        EXPECT_EQ(option.or_expect(Lit("some message")), 10);
    
        Option<u32> bad_option = Option<u32>::make_none();
        EXPECT_DEATH(bad_option.or_expect(Lit("some message")), "some message");
    }
    
    TEST(Option, destroy_contents) {
        Option<u32> option = Option<u32>::make_some(10);
        option.destroy_contents([](u32 value) {
            EXPECT_EQ(value, 10);
        });
    }
}

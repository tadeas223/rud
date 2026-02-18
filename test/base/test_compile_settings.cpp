#include <gtest/gtest.h>
#include "rud/base/string.hpp"
#include "rud/base/compile_settings.hpp"

using namespace rud;

//
// basic behavior
//

TEST(Assert, TrueDoesNotAbort) {
    // should not terminate when condition is true
    _assert(true, Lit("true_expr"), Lit("message"));
}

TEST(Assert, MultipleTrueValuesDoNotAbort) {
    // run multiple times to ensure no hidden state
    for (u32 i = 0; i < 100; ++i) {
        _assert(i < 200, Lit("loop_true"), Lit("message"));
    }
}

//
// failure behavior
//

TEST(Assert, FalseTriggersDeath) {
    EXPECT_DEATH(
        _assert(false, Lit("false_expr"), Lit("message")),
        "."
    );
}

TEST(Assert, FalseTriggersDeathForVariousInputs) {
    // test several failing inputs to ensure consistent behavior
    for (u32 i = 0; i < 32; ++i) {
        EXPECT_DEATH(
            _assert(i > 1000, Lit("loop_false"), Lit("message")),
            "."
        );
    }
}

//
// string view handling
//

TEST(Assert, AcceptsDifferentStringViews) {
    StringView expr = StringView::make("expr", 4);
    StringView msg  = StringView::make("msg", 3);

    _assert(true, expr, msg);
}

TEST(Assert, AcceptsEmptyMessages) {
    _assert(true, Lit("expr"), StringView::make("", 0));
}

//
// edge cases
//

TEST(Assert, HandlesLongMessagesWithoutCrash) {
    const char* long_msg =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    _assert(true, Lit("expr"), StringView::make_cstr(long_msg));
}


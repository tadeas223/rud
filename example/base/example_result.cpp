#include "rud/base/result.hpp"
#include "rud/os/platform/std_out.hpp"

using namespace rud;

// declare a custom error type
enum class MathError {
    DivideByZero,
};

Result<f32, MathError> divide(f32 x, f32 y) {
    if(y == 0) {
        // return an error
        return Result<f32, MathError>::make_error(MathError::DivideByZero);
    }
    
    // return a value and no error
    return Result<f32, MathError>::make_ok(x/y);
}

int main (int argc, char *argv[]) {
    // this will fail
    // its good to prefix the result type with r_
    auto r_value = divide(10, 0);
    
    // get a value or calculate a default value in a function
    f32 value_or_try = r_value.or_try([]() {
            os::debug_print(Lit("division failed, trying y=1\n"));

            return divide(10, 1).or_panic();
    });
    
    // get a value of a default one
    f32 value_or_default = r_value.or_default(10);
    
    // check whether the reuslt has an error
    if(r_value.is_error()) {
        // unwrap error will panic in safer_mode if used on a ok result
        if(r_value.unwrap_error() == MathError::DivideByZero) {
            os::debug_print(Lit("divided by zero catched\n"));
        }
    } else {
        // now that the error is check the value can be safely unwraped
        // if unwrap() is called on a error result in safer_mode it will panic
        f32 value = r_value.unwrap();
    }


    
    // this will not fail
    auto r_value2 = divide(10, 1);

    // get a value or panic
    f32 value2_or_panic = r_value2.or_panic();
    
    // get a value or panic with the specified message
    f32 value2_or_expect = r_value2.or_expect(Lit("failed to divide\n"));
    return 0;
}

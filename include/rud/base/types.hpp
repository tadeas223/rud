#ifndef RUD_BASE_TYPES_HPP
#define RUD_BASE_TYPES_HPP

#include <type_traits>

namespace rud {
    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long;
    
    using s8 = signed char;
    using s16 = signed short;
    using s32 = signed int;
    using s64 = signed long;
    
    using f32 = float;
    using f64 = double;

    using ascii = char;

    template<typename T>
    using callback_destroy = void (*)(void* ctx, T value);

    template<typename R, typename...Args>
        concept concept_invokable = std::is_invocable_r_v<R, Args...>;
}

#endif

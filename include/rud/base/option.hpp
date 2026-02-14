#ifndef RUD_BASE_OPTION_HPP
#define RUD_BASE_OPTION_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/string.hpp"
#include "rud/base/system.hpp"

namespace rud {
    template<typename V>
    struct [[nodiscard]] Option {
        V value;
        bool some;
        
        static inline Option<V> make_some(V value) {
            return {value, true}; 
        }
        
        static inline Option<V> make_none() {
            return {.some = false}; 
        }

        inline V or_panic() {
            if(!some) panic(Lit("option had no value"));
            return value;
        }

        inline V unwrap() {
            Assert(!some, Lit("Option::unwrap called, but option does not hold a value"));
            return value;
        }
    };
}

#endif

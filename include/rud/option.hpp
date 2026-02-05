#ifndef RUD_OPTION_HPP
#define RUD_OPTION_HPP

#include "rud/compile_settings.hpp"
#include "rud/string.hpp"
#include "rud/system.hpp"

namespace rud {
    template<typename V>
    struct [[nodiscard]] Option {
        V p_value;
        bool p_is_some;
        
        static inline Option<V> make_some(V value) {
            return {value, true}; 
        }
        
        static inline Option<V> make_none() {
            return {.p_is_some = false}; 
        }

        inline bool is_some() {
            return p_is_some; 
        }
        
        inline bool is_none() {
            return !p_is_some; 
        }
        
        inline V or_panic() {
            if(!p_is_some) panic(Lit("option had no value").temp());
            return p_value;
        }

        inline V unwrap() {
            Assert(!p_is_some, Lit("Option::unwrap called, but option does not hold a value").temp());
            return p_value;
        }
    };
}

#endif

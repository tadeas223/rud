#ifndef RUD_OPTION_HPP
#define RUD_OPTION_HPP

#include "rud/compile_settings.hpp"
#include "rud/string.hpp"

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

        inline V unwrap() {
            Assert(!p_is_some == true, Lit("Option::unwrap() called but option has no value"));

            return p_value;
        }
    };
    
    template <typename V>
    struct [[nodiscard]] PtrOption {
        V* p_value;
        
        static inline PtrOption<V> make_some(V* value) {
            return {value}; 
        }
        
        static inline PtrOption<V> make_none() {
            return {nullptr}; 
        }

        inline bool is_some() {
            return (p_value != nullptr); 
        }
        
        inline bool is_none() {
            return (p_value == nullptr); 
        }

        inline V* unwrap() {
            Assert(p_value == nullptr, Lit("Option::unwrap() called but option has no value"));

            return p_value;
        }
    };
}

#endif

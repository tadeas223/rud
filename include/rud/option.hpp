#ifndef RUD_OPTION_HPP
#define RUD_OPTION_HPP

namespace rud {
    template<typename V>
    struct [[nodiscard]] Option {
        V value;
        bool has_value;
        
        static inline Option<V> make_some(V value) {
            return {value, true}; 
        }
        
        static inline Option<V> make_none() {
            return {.has_value = false}; 
        }
    };
}

#endif

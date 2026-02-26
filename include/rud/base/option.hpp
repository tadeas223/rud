#ifndef RUD_BASE_OPTION_HPP
#define RUD_BASE_OPTION_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/destroyer.hpp"
#include "rud/base/string.hpp"
#include "rud/base/system.hpp"

namespace rud {
    template<typename V>
    struct [[nodiscard]] Option {
        V p_value;
        bool p_has_some;
        
// {{{ get_set
        inline bool has_some() {
            return p_has_some;
        }
        
        inline bool has_none() {
            return !p_has_some;
        }
// }}}

// {{{ make_destroy
        static inline Option<V> make_some(V value) {
            return {value, true};
        }
        
        static inline Option<V> make_none() {
            return {.p_has_some = false};
        }
        
        inline void destroy_contents(Destroyer<V> destroyer) {
            if(p_has_some) {
                destroyer.invoke(p_value);
            }
        }
// }}}

        inline V or_panic() {
            if(!p_has_some) panic(Lit("option had no value"));
            return p_value;
        }

        inline V or_expect(StringView msg) {
            if(!p_has_some) panic_raw(msg);
            return p_value;
        }

        inline V unwrap() {
            Pre(has_some());
            
            return p_value;
        }
    };
}

#endif

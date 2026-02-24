#ifndef RUD_BASE_RESULT_HPP
#define RUD_BASE_RESULT_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/system.hpp"
#include "rud/base/string.hpp"
#include "rud/base/types.hpp"

namespace rud {
    template<typename V, typename E>
    struct [[nodiscard]] Result {
        union {
            V p_value;
            E p_error;
        };

        bool p_is_ok;
        
// {{{ get_set
        inline bool is_ok() {
            return p_is_ok;
        }
        
        inline bool is_error() {
            return !p_is_ok;
        }
// }}}

// {{{ make_destroy
        inline static Result<V, E> make_ok(V value) {
            return {.p_value = value, .p_is_ok = true};
        }
        
        inline static Result<V, E> make_error(E error) {
            return {.p_error = error, .p_is_ok = false};
        }

        inline void destroy_contents(callback_destroy<V> destroy_func) {
            if(is_ok()) {
                destroy_func(p_value);
            }
        }

        inline void destroy_error(callback_destroy<E> destroy_func) {
            if(is_error()) {
                destroy_func(p_error);
            }
        }
// }}}
       
        inline V or_expect(StringView msg) {
            if(is_error()) panic_raw(msg);
            return p_value;
        }
    
        template<typename F>
        inline V or_try(F func) {
            if(is_ok()) {
                return p_value;
            } else {
                return func();
            }
        }

        inline V or_panic() {
            if(is_error()) panic(Lit("unhandled error occured"));
            return p_value;
        }
 
        V unwrap() {
            Pre(is_ok());

            return p_value;
        }

        E unwrap_error() {
            Pre(is_error());

            return p_error;
        }
    };
    
    
    template<typename E>
    struct [[nodiscard]] Result<void, E> {
        E p_error;
        bool p_is_ok;
       
// {{{ get_set
        inline bool is_ok() {
            return p_is_ok;
        }

        inline bool is_error() {
            return !p_is_ok;
        }
// }}}

// {{{ make_destroy
        inline static Result<void, E> make_ok() {
            return {.p_is_ok = true};
        }
        
        inline static Result<void, E> make_error(E error) {
            return {.p_error = error, .p_is_ok = false};
        }

        inline void destroy_error(callback_destroy<E> destroy_func) {
            if(is_error()) {
                destroy_func(p_error);
            }
        }
// }}}

        inline void or_expect(StringView msg) {
            if(is_error()) panic_raw(msg);
        }
        
        template<typename F>
        inline void or_try(F func) {
            if(is_error()) {
                return func();
            }
        }
        
        inline void or_panic() {
            if(is_error()) panic(Lit("unhandled error occured"));
        }

        void unwrap() {
            Pre(is_ok());
        }

        E unwrap_error() {
            Pre(is_error());

            return p_error;
        }
    };
}

#endif

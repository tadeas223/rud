#ifndef RUD_RESULT_HPP
#define RUD_RESULT_HPP

#include "rud/compile_settings.hpp"
#include "rud/system.hpp"
#include "rud/string.hpp"

namespace rud {
    template<typename V, typename E>
    struct [[nodiscard]] Result {
        union {
            V p_value;
            E p_error;
        };

        bool p_is_ok;

        inline static Result<V, E> make_ok(V value) {
            return {.p_value = value, .p_is_ok = true};
        }
        
        inline static Result<V, E> make_error(E error) {
            return {.p_error = error, .p_is_ok = false};
        }
       
        template<typename Lambda>
        inline V try_or(Lambda l) {
            if(!p_is_ok) return l();
            return p_value;
        }

        inline V except(const String* msg) {
            if(!p_is_ok) panic(msg);
            return p_value;
        }

        inline V or_panic() {
            if(!p_is_ok) panic(Lit("unhandled error occured").temp());
            return p_value;
        }
 
        inline bool is_ok() {
            return p_is_ok; 
        }
        
        inline bool is_error() {
            return !p_is_ok; 
        }

        V unwrap() {
            Assert(p_is_ok == true, Lit("Result::value() called, but result contained an error").temp());

            return p_value;
        }

        E unwrap_error() {
            Assert(p_is_ok == false, Lit("Result::error() called, but result contained a value").temp());

            return p_error;
        }
    };
    
    
    template<typename E>
    struct [[nodiscard]] Result<void, E> {
        E p_error;
        bool p_is_ok;
        
        inline static Result<void, E> make_ok() {
            return {.p_is_ok = true};
        }
        
        inline static Result<void, E> make_error(E error) {
            return {.p_error = error, .p_is_ok = true};
        }

        inline void except(const String* msg) {
            if(!p_is_ok) panic(msg);
        }
        
        template<typename Lambda>
        inline void try_or(Lambda l) {
            if(!p_is_ok) l();
        }

        inline bool is_ok() {
            return p_is_ok; 
        }

        inline bool is_error() {
            return !p_is_ok; 
        }

        inline void or_panic() {
            if(!p_is_ok) panic(Lit("unhandled error occured").temp());
        }

        void unwrap() {
            Assert(p_is_ok == true, Lit("Result::value() called, but result contained an error").temp());
        }

        E unwrap_error() {
            Assert(p_is_ok == false, Lit("Result::error() called, but result contained a value").temp());

            return p_error;
        }
    };
}

#endif

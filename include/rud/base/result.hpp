#ifndef RUD_BASE_RESULT_HPP
#define RUD_BASE_RESULT_HPP

#include "rud/base/compile_settings.hpp"
#include "rud/base/system.hpp"
#include "rud/base/string.hpp"

namespace rud {
    template<typename V, typename E>
    struct [[nodiscard]] Result {
        union {
            V value;
            E error;
        };

        bool ok;

        inline static Result<V, E> make_ok(V value) {
            return {.value = value, .ok = true};
        }
        
        inline static Result<V, E> make_error(E error) {
            return {.error = error, .ok = false};
        }
       
        template<typename Lambda>
        inline V try_or(Lambda l) {
            if(!ok) return l();
            return value;
        }

        inline V except(const String msg) {
            if(!ok) panic(msg);
            return value;
        }

        inline V or_panic() {
            if(!ok) panic(Lit("unhandled error occured"));
            return value;
        }
 
        V unwrap() {
            Assert(ok == true, Lit("Result::value() called, but result contained an error"));

            return value;
        }

        E unwrap_error() {
            Assert(ok == false, Lit("Result::error() called, but result contained a value"));

            return error;
        }
    };
    
    
    template<typename E>
    struct [[nodiscard]] Result<void, E> {
        E error;
        bool ok;
        
        inline static Result<void, E> make_ok() {
            return {.ok = true};
        }
        
        inline static Result<void, E> make_error(E error) {
            return {.error = error, .ok = true};
        }

        inline void except(const String msg) {
            if(!ok) panic(msg);
        }
        
        template<typename Lambda>
        inline void try_or(Lambda l) {
            if(!ok) l();
        }

        inline void or_panic() {
            if(!ok) panic(Lit("unhandled error occured"));
        }

        void unwrap() {
            Assert(ok == true, Lit("Result::value() called, but result contained an error"));
        }

        E unwrap_error() {
            Assert(ok == false, Lit("Result::error() called, but result contained a value"));

            return error;
        }
    };
}

#endif

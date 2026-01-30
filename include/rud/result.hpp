#ifndef RUD_RESULT_HPP
#define RUD_RESULT_HPP

#include "rud/system.hpp"
#include "rud/string.hpp"

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

        inline V except(const String& msg) {
            if(!ok) panic(msg);
            return value;
        }
    };
    
    
    template<typename E>
    struct [[nodiscard]] Result<void, E> {
        union {
            E error;
        };

        bool ok;
    
        inline static Result<void, E> make_ok() {
            return {.ok = true};
        }
        
        inline static Result<void, E> make_error(E error) {
            return {.error = error, .ok = false};
        }

        inline void except(const String& msg) {
            if(!ok) panic(msg);
        }
        
        template<typename Lambda>
        inline void try_or(Lambda l) {
            if(!ok) l();
        }
    };


}

#endif

#ifndef RUD_STRING_HPP
#define RUD_STRING_HPP

#include "rud/macros.hpp"
#include "rud/types.hpp"

#define Lit(cstr) rud::StringLit::make(cstr)

namespace rud {
    struct String {
        ascii* chars;
        u32 len;

        ascii* to_cstr() const;
    };
    
    struct AllocString : String {
        //  debug_mode:
        //      cstr != null
        //  
        //  not checked: cstr must be null terminated
        static AllocString make_copy_cstr(const ascii* cstr);
        
        static AllocString make_copy(const ascii* chars, u32 len);
        
        static AllocString make_take(const ascii* chars, u32 len);
        
        template<u32 N>
        static constexpr AllocString make_copy(const ascii (&s)[N]) {
            return make_copy(s, N-1);
        }

        void destroy() const;
    };
    
    struct StringLit: String {
        template<u32 N>
        static constexpr StringLit make(const ascii (&s)[N]) {
            return {const_cast<ascii*>(s), N-1};
        }

        static StringLit make(const ascii* buffer, const u32 len);
        static StringLit make_cstr(const ascii* cstr);
    };

    u32 cstr_len(const ascii* cstr);
}

#endif

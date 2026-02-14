#ifndef RUD_BASE_STRING_HPP
#define RUD_BASE_STRING_HPP

#include "rud/base/types.hpp"
#include "rud/ds/linear_view.hpp"

#define Lit(cstr) rud::StringLit::make(cstr)

namespace rud {
    struct AllocString;

    struct String {
        ascii* chars;
        u32 len;

        ascii* to_cstr() const;
    };
    
    struct AllocString : String {
        bool deallocate_on_destroy = true;

        //  debug_mode:
        //      cstr != null
        //  
        //  not checked: cstr must be null terminated
        static AllocString make_copy_cstr(const ascii* cstr);
        
        static AllocString make_copy(const ascii* chars, u32 len);
        static AllocString make_copy(String str);
        
        static AllocString make_take(const ascii* chars, u32 len);

        void push_copy(String str);
        
        template<u32 N>
        static constexpr AllocString make_copy(const ascii (&s)[N]) {
            return make_copy(s, N-1);
        }
        
        void destroy() const;
    };
    
    struct StringLit: String {
        static StringLit make(const ascii* buffer, const u32 len);
        
        template<u32 N>
        static constexpr StringLit make(const ascii (&s)[N]) {
            return {const_cast<ascii*>(s), N-1};
        }

        static StringLit make_cstr(const ascii* cstr);
    };

    u32 cstr_len(const ascii* cstr);

    AllocString string_join(ds::LinearView<String> strings);
}

#endif

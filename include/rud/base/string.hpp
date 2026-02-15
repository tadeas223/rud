#ifndef RUD_BASE_STRING_HPP
#define RUD_BASE_STRING_HPP

#include "rud/base/types.hpp"

#define Lit(cstr) rud::StringView::make(cstr)

namespace rud {
    struct String {
        ascii* chars;
        u32 len;

        ascii* to_cstr() const;

        inline bool operator==(String rhs) const {
            for(u32 i = 0; i < len; i++) {
                if(rhs.chars[i] != chars[i]) {
                    return false;
                }
            }
            return true;
        }
    };
    
    struct StringView: String {
        static StringView make(const ascii* buffer, u32 len);
        
        template<u32 N>
        static constexpr StringView make(const ascii (&s)[N]) {
            return {const_cast<ascii*>(s), N-1};
        }

        static StringView make_cstr(const ascii* cstr);
    };
    
    struct C_StringAlloc : String {
        bool deallocate_on_destroy = true;

        static C_StringAlloc make_copy_cstr(const ascii* cstr);
        
        static C_StringAlloc make();
        
        static C_StringAlloc make_copy(const ascii* chars, u32 len);
        static C_StringAlloc make_copy(StringView str);
        
        static C_StringAlloc make_take(const ascii* chars, u32 len);

        const C_StringAlloc* push_copy(StringView str);
        
        template<u32 N>
        static constexpr C_StringAlloc make_copy(const ascii (&s)[N]) {
            return make_copy(s, N-1);
        }

        inline StringView view() const {
            return StringView::make(chars, len);
        }
        
        void destroy();
    };

    u32 cstr_len(const ascii* cstr);
}

#endif

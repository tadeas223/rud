#include "rud/base/string.hpp"

#include "rud/base/memory.hpp"

namespace rud {
    C_StringAlloc C_StringAlloc::make_copy_cstr(const ascii* cstr){
        u32 len = cstr_len(cstr);
        ascii* new_chars = static_cast<ascii*>(allocate_size(len));

        mem_copy(new_chars, cstr, len);

        return {{new_chars, len}};
    }

    C_StringAlloc C_StringAlloc::make_copy(const ascii* chars, u32 len) {
        ascii* new_chars = static_cast<ascii*>(allocate_size(len));

        mem_copy(new_chars, chars, len);

        return {{new_chars, len}};
    }
    
    C_StringAlloc C_StringAlloc::make_copy(const StringView str) {
        return make_copy(str.chars, str.len);
    }
    
    C_StringAlloc C_StringAlloc::make_take(const ascii* chars, u32 len) {
        return {{const_cast<ascii*>(chars), len}}; 
    }
        
    C_StringAlloc C_StringAlloc::make() {
        return {{nullptr, 0}};
    }
        
    const C_StringAlloc* C_StringAlloc::push_copy(const StringView str) {
        chars = static_cast<ascii*>(reallocate(chars, len + str.len));
        mem_copy(chars + len, str.chars, str.len);
        len += str.len;
        return this;
    }

    void C_StringAlloc::destroy() {
        if(deallocate_on_destroy) {
            deallocate(chars);
        }
    }

    StringView StringView::make_cstr(const ascii* cstr) {
        u32 len = cstr_len(cstr);

        return {const_cast<ascii*>(cstr), len};
    }

    StringView StringView::make(const ascii* buffer, const u32 len){
        return {const_cast<ascii*>(buffer), len};
    }

    ascii* String::to_cstr() const {
        ascii* cstr = static_cast<ascii*>(allocate_size(len+1));

        mem_copy(cstr, chars, len);
        cstr[len] = '\0';

        return cstr;
    }
   
    u32 cstr_len(const ascii* cstr) {
        u32 len = 0;
        while(cstr[len]) {
            len++;
        }

        return len;
    }
}

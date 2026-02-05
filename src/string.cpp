#include "rud/string.hpp"
#include "rud/memory.hpp"

namespace rud {
    AllocString AllocString::make_copy_cstr(const ascii* cstr){
        u32 len = cstr_len(cstr);
        ascii* new_chars = static_cast<ascii*>(allocate_size(len));

        mem_copy(new_chars, cstr, len);

        return {new_chars, len};
    }

    AllocString AllocString::make_copy(const ascii* chars, const u32 len) {
        ascii* new_chars = static_cast<ascii*>(allocate_size(len));

        mem_copy(new_chars, chars, len);

        return {new_chars, len};
    }
    
    AllocString AllocString::make_take(const ascii* chars, u32 len) {
        return {const_cast<ascii*>(chars), len}; 
    }

    void AllocString::destroy() const {
        deallocate(chars);
    }

    StringLit StringLit::make_cstr(const ascii* cstr) {
        u32 len = cstr_len(cstr);

        return {const_cast<ascii*>(cstr), len};
    }

    StringLit StringLit::make(const ascii* buffer, const u32 len){
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

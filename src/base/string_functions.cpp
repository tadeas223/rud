#include "rud/base/string_functions.hpp"

using namespace rud::ds;

namespace rud {
    C_StringAlloc string_join(ds::LinearView<const StringView> strings) {
        C_StringAlloc str = C_StringAlloc::make_copy(*strings[0]);
        for(u32 i = 1; i < strings.len(); i++) {
            str.push_copy(*strings[i]);
        }

        return str;
    }
    
    ds::C_DArray<StringView> string_split(StringView string, ascii sepparator) {
        C_DArray<StringView> splits = C_DArray<StringView>::make();
    
        u32 split_start = 0;

        for(u32 i = 0; i < string.len; i++) {
            if(string.chars[i] == sepparator) {
                StringView split = StringView::make(string.chars + split_start, i - split_start);
                splits.push(split);
                split_start = i + 1; // skip the sepparator character
            }
        }
        
        StringView split = StringView::make(string.chars + split_start, string.len - split_start);
        splits.push(split);
        
        return splits;
    }
}

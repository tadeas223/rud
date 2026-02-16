#ifndef RUD_BASE_STRING_FUNCTIONS_HPP
#define RUD_BASE_STRING_FUNCTIONS_HPP

#include "rud/base/string.hpp"
#include "rud/ds/c_vector.hpp"

namespace rud {
    C_StringAlloc string_join(ds::LinearView<StringView> strings);
    ds::C_Vector<StringView> string_split(StringView string, ascii sepparator);
}

#endif

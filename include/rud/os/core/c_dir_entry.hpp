#ifndef RUD_OS_CORE_DIR_ENTRY_HPP
#define RUD_OS_CORE_DIR_ENTRY_HPP

#include "rud/base/string.hpp"
#include "rud/os/core/dir_entry_type.hpp"

namespace rud::os {
    struct C_DirEntry {
        C_StringAlloc p_name;
        DirEntryType type;
        
        inline const C_StringAlloc name() const {
            return p_name;
        }

        inline void destroy() {
            p_name.destroy();
        }
    };
}

#endif

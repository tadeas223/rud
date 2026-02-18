#ifndef RUD_OS_CORE_FILE_CREATE_MODE_HPP
#define RUD_OS_CORE_FILE_CREATE_MODE_HPP

#include "rud/base/types.hpp"

namespace rud::os {
    enum class FileCreateMode : u32 {
        Create = 0x01,
        Truncate = 0x2,
    };

    inline FileCreateMode operator|(FileCreateMode a, FileCreateMode b) {
        return static_cast<FileCreateMode>(static_cast<u32>(a) | static_cast<u32>(b)); 
    }
    
    inline bool operator*(FileCreateMode a, FileCreateMode b) {
        return (static_cast<u32>(a) * static_cast<u32>(b)) != 0; 
    }
}

#endif

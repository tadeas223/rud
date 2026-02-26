#ifndef RUD_OS_CORE_FILE_ACCESS_MODE_HPP
#define RUD_OS_CORE_FILE_ACCESS_MODE_HPP

#include "rud/base/types.hpp"

namespace rud::os {
    enum class FileAccessMode : u32 {
        Read = 0x01, Write = 0x02, Append = 0x04, 
    };

    inline FileAccessMode operator|(FileAccessMode a, FileAccessMode b) {
        return static_cast<FileAccessMode>(static_cast<u32>(a) | static_cast<u32>(b)); 
    }
    
    inline bool operator*(FileAccessMode a, FileAccessMode b) {
        return (static_cast<u32>(a) * static_cast<u32>(b)) != 0; 
    }
}

#endif

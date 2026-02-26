#ifndef RUD_OS_CORE_DIRECTORY_CREATE_MODE_HPP
#define RUD_OS_CORE_DIRECTORY_CREATE_MODE_HPP

#include "rud/base/types.hpp"

namespace rud::os {
    enum class DirectoryCreateMode : u32 {
        Create = 0x01,
        CreateIfDoesntExist = 0x02,
    };
    
    inline DirectoryCreateMode operator|(DirectoryCreateMode a, DirectoryCreateMode b) {
        return static_cast<DirectoryCreateMode>(static_cast<u32>(a) | static_cast<u32>(b)); 
    }
    
    inline bool operator*(DirectoryCreateMode a, DirectoryCreateMode b) {
        return (static_cast<u32>(a) * static_cast<u32>(b)) != 0; 
    }
}

#endif

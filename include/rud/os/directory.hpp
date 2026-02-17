#ifndef RUD_OS_DIRECTORY_HPP
#define RUD_OS_DIRECTORY_HPP

#include "rud/os_low/io.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct C_Directory {
        os_low::C_DirectoryHandle handle;

        static Result<C_Directory, os_low::IOError> make(StringView path);
        static Result<C_Directory, os_low::IOError> make(StringView path, os_low::DirectoryCreateMode create_mode);
        
        Result<ds::C_DArray<os_low::C_DirEntry>, os_low::IOError> get_entries(u32 entry_count);        
        
        static Result<void, os_low::IOError> set_current_directory(C_Directory* directory);
        static Result<void, os_low::IOError> set_current_directory(StringView path);
        static Result<C_Directory, os_low::IOError> get_current_directory();

        void destroy();
    };
}

#endif

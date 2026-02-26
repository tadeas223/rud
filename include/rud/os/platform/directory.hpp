#ifndef RUD_OS_PLATFORM_DIRECTORY_HPP
#define RUD_OS_PLATFORM_DIRECTORY_HPP

#include "rud/ds/c_darray.hpp"
#include "rud/os/core/io_error.hpp"
#include "rud/os/core/c_dir_entry.hpp"
#include "rud/os/core/directory_create_mode.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct C_Directory {
        void* handle;

        static Result<C_Directory, IOError> make(StringView path);
        static Result<C_Directory, IOError> make(StringView path, DirectoryCreateMode create_mode);

        
        Result<ds::C_DArray<C_DirEntry>, IOError> get_entries(u32 entry_count);        
        
        static Result<void, IOError> set_current_directory(C_Directory* directory);
        static Result<void, IOError> set_current_directory(StringView path);
        static Result<C_Directory, IOError> get_current_directory();

        void destroy();
    };
}

#endif

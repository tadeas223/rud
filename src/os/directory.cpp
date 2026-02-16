#include "rud/os/directory.hpp"
#include "rud/os_low/io.hpp"

using namespace rud;
using namespace rud::os_low;
using namespace rud::os;

namespace rud::os {
    Result<C_Directory, os_low::IOError> C_Directory::make(StringView path) {
        Result<C_DirectoryHandle, IOError> r_handle = c_directory_handle_make(path);
        if(!r_handle.ok) {
            return Result<C_Directory, IOError>::make_error(r_handle.unwrap_error());
        }
        C_DirectoryHandle handle = r_handle.unwrap();

        return Result<C_Directory, IOError>::make_ok({handle});
    }

    Result<C_Directory, os_low::IOError> C_Directory::make(StringView path, os_low::DirectoryCreateMode create_mode) {
        Result<C_DirectoryHandle, IOError> r_handle = c_directory_handle_make(path, create_mode);
        if(!r_handle.ok) {
            return Result<C_Directory, IOError>::make_error(r_handle.unwrap_error());
        }
        C_DirectoryHandle handle = r_handle.unwrap();

        return Result<C_Directory, IOError>::make_ok({handle});
    }
    
    Result<ds::C_DArray<os_low::C_DirEntry>, os_low::IOError> C_Directory::get_entries(u32 entry_count) {
        return c_directory_handle_get_entries(&handle, entry_count);
    }

    void C_Directory::destroy() {
        c_directory_handle_destroy(&handle);
    }
}

#include "rud/os/file.hpp"

#include "rud/os_low/io.hpp"

using namespace rud::os_low;
using namespace rud;

namespace rud::os {
    Result<C_File, os_low::IOError> C_File::make(StringView path, os_low::FileAccessMode access_mode) {
        Result<C_FileHandle, IOError> r_handle = c_file_handle_make(path, access_mode);
        if(!r_handle.ok) {
            return Result<C_File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<C_File, IOError>::make_ok(C_File{.handle = r_handle.unwrap() });
    }
    
    Result<C_File, os_low::IOError> C_File::make(const C_Directory* dir, StringView path, os_low::FileAccessMode access_mode, os_low::FileCreateMode create_mode) {
        Result<C_FileHandle, IOError> r_handle = c_file_handle_make(&dir->handle, path, access_mode, create_mode);
        if(!r_handle.ok) {
            return Result<C_File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<C_File, IOError>::make_ok(C_File{.handle = r_handle.unwrap() });
    }
    
    Result<C_File, os_low::IOError> C_File::make(const C_Directory* dir, StringView path, os_low::FileAccessMode access_mode) {
        Result<C_FileHandle, IOError> r_handle = c_file_handle_make(&dir->handle, path, access_mode);
        if(!r_handle.ok) {
            return Result<C_File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<C_File, IOError>::make_ok(C_File{.handle = r_handle.unwrap() });
    }


    Result<C_File, os_low::IOError> C_File::make(StringView path, os_low::FileAccessMode access_mode, os_low::FileCreateMode create_mode) {
        Result<C_FileHandle, IOError> r_handle = c_file_handle_make(path, access_mode, create_mode);
        if(!r_handle.ok) {
            return Result<C_File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<C_File, IOError>::make_ok(C_File{.handle = r_handle.unwrap() });
    }
    
    Result<u64, os_low::IOError> C_File::read(void* buffer, u64 size) {
        return c_file_handle_read(&handle, buffer, size);
    }
    
    Result<u64, os_low::IOError> C_File::write(const void* buffer, u64 size) {
        return c_file_handle_write(&handle, buffer, size);
    }
    
    Result<void, os_low::IOError> C_File::seek(os_low::FileSeekFrom from, u64 bytes) {
        return c_file_handle_seek(&handle, from, bytes);
    }
    Result<os_low::FileMetadata, os_low::IOError> C_File::metadata() {
        return c_file_handle_metadata(&handle);
    }

    void C_File::destroy() {
        c_file_handle_destroy(&handle);
    }

    
}

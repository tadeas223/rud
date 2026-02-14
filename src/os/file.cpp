#include "rud/os/file.hpp"
#include "rud/os_low/io.hpp"

using namespace rud::os_low;
using namespace rud;

namespace rud::os {
    Result<File, os_low::IOError> File::make(const String path, os_low::FileAccessMode access_mode) {
        Result<FileHandle, IOError> r_handle = file_handle_make(path, access_mode);
        if(!r_handle.ok) {
            return Result<File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<File, IOError>::make_ok(File{.p_handle = r_handle.unwrap() });
    }

    Result<File, os_low::IOError> File::make(const String path, os_low::FileAccessMode access_mode, os_low::FileCreateMode create_mode) {
        Result<FileHandle, IOError> r_handle = file_handle_make(path, access_mode, create_mode);
        if(!r_handle.ok) {
            return Result<File, IOError>::make_error(r_handle.unwrap_error());
        }

        return Result<File, IOError>::make_ok(File{.p_handle = r_handle.unwrap() });
    }
    
    Result<u64, os_low::IOError> File::read(void* buffer, u64 size) {
        return file_handle_read(&p_handle, buffer, size);
    }
    
    Result<u64, os_low::IOError> File::write(const void* buffer, u64 size) {
        return file_handle_write(&p_handle, buffer, size);
    }
    
    Result<void, os_low::IOError> File::seek(os_low::FileSeekFrom from, u64 bytes) {
        return file_handle_seek(&p_handle, from, bytes);
    }
    Result<os_low::FileMetadata, os_low::IOError> File::metadata() {
        return file_handle_metadata(&p_handle);
    }

    void File::destroy() {
        file_handle_destroy(&p_handle);
    }

    
}

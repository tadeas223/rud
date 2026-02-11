#include "rud/os_low/io.hpp"

#include "rud/base/memory.hpp"
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace rud;
using namespace rud::os_low;

namespace {
    constexpr u32 new_file_permissions = 0644;

    int file_access_mode_to_flags(FileAccessMode mode) {
        int flags = 0;
        if(mode * FileAccessMode::Read && mode * FileAccessMode::Write) {
            flags |= O_RDWR;
        } else if(mode * FileAccessMode::Read) {
            flags |= O_RDONLY; 
        } else if(mode * FileAccessMode::Write) {
            flags |= O_WRONLY;
        } else {
            return -1; 
        }

        if(mode * FileAccessMode::Append) {
            flags |= O_APPEND; 
        }

        return flags;
    }
    
    int file_make_mode_to_flags(FileCreateMode mode) {
        int flags = 0;
        if(mode * FileCreateMode::Truncate) {
            flags |= O_TRUNC; 
        }
        
        if(mode * FileCreateMode::Create) {
            flags |= O_CREAT; 
        }
        
        return flags;
    }
}

namespace rud::os {
    struct InternalFileHandle {
        int descriptor;
    };

    Result<FileHandle, IOError> file_handle_make(const String path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<FileHandle, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<FileHandle, IOError>::make_ok(handle);
    }
    
    Result<FileHandle, IOError> file_handle_make(const String path, FileAccessMode access_mode, FileCreateMode make_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if(access_flags < 0) { 
            return Result<FileHandle, IOError>::make_error(IOError::InvalidInput);
        }

        int make_flags = file_make_mode_to_flags(make_mode);

        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags|make_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<FileHandle, IOError>::make_ok(handle);
    }
    
    Result<u64, IOError> file_handle_read(FileHandle* handle, void* buffer, u64 size) {
        ssize_t read_result = ::read(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(read_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(read_result);
    }

    Result<u64, IOError> file_handle_write(FileHandle* handle, const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
    
    Result<void, IOError> file_handle_seek(FileHandle* handle, FileSeekFrom from, u64 bytes) {
        int whence;
        switch(from) {
            case FileSeekFrom::Current:
                whence = SEEK_CUR;
                break;
            case FileSeekFrom::Start:
                whence = SEEK_SET;
                break;
            case FileSeekFrom::End:
                whence = SEEK_END;
                break;
        }

        off_t lseek_result = lseek(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, bytes, whence);
        if(lseek_result < 0) {
            return Result<void, IOError>::make_error(IOError::Other); 
        }

        return Result<void, IOError>::make_ok();
    }

    void file_handle_destroy(FileHandle* handle) {
        int close_result = close(reinterpret_cast<InternalFileHandle*>(handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a file"));
        }
        deallocate(handle);
    }
    
    Result<FileMetadata, IOError> file_handle_metadata(FileHandle* handle) {
        FileMetadata metadata;
        
        struct stat st;
        if(fstat(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, &st) < 0) {
            return Result<FileMetadata, IOError>::make_error(IOError::Other);
        }

        metadata.size = st.st_size;
        
        return Result<FileMetadata, IOError>::make_ok(metadata);
    }
    
    InternalFileHandle internal_std_in = {
        .descriptor = 0
    };

    InternalFileHandle internal_std_out = {
        .descriptor = 1
    };

    InternalFileHandle internal_std_err = {
        .descriptor = 2
    };

    StdStreamHandle std_in_handle = &internal_std_in;
    StdStreamHandle std_out_handle = &internal_std_out;
    StdStreamHandle std_err_handle = &internal_std_err;

    Result<u64, IOError> std_stream_handle_read(StdStreamHandle* handle, void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> std_stream_handle_write(StdStreamHandle* handle, const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
}

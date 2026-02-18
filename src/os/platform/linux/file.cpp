#include "rud/os/platform/file.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace rud::os;
using namespace rud;

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
    
    int file_create_mode_to_flags(FileCreateMode mode) {
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

    Result<C_File, IOError> C_File::make(StringView path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_File, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_File, IOError>::make_ok({.handle = handle});
    }
    
    Result<C_File, IOError> C_File::make(C_Directory* dir, StringView path, FileAccessMode access_mode, FileCreateMode create_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_File, IOError>::make_error(IOError::InvalidInput);
        }
        
        int create_flags = file_create_mode_to_flags(create_mode);
        
        ascii* cstr = path.to_cstr();
        int open_result = openat(reinterpret_cast<InternalFileHandle*>(dir->handle)->descriptor, cstr, access_flags|create_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_File, IOError>::make_ok({.handle = handle});
    }
    
    Result<C_File, IOError> C_File::make(C_Directory* dir, StringView path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_File, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = openat(reinterpret_cast<InternalFileHandle*>(dir->handle)->descriptor, cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_File, IOError>::make_ok({.handle = handle});
    }


    Result<C_File, IOError> C_File::make(StringView path, FileAccessMode access_mode, FileCreateMode create_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if(access_flags < 0) { 
            return Result<C_File, IOError>::make_error(IOError::InvalidInput);
        }

        int create_flags = file_create_mode_to_flags(create_mode);

        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags|create_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_File, IOError>::make_ok({.handle = handle});
    }
    
    Result<u64, IOError> C_File::read(void* buffer, u64 size) {
        ssize_t read_result = ::read(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(read_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(read_result);
    }
    
    Result<u64, IOError> C_File::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
    
    Result<void, IOError> C_File::seek(FileSeekFrom from, u64 bytes) {
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
            default: [[unlikely]] 
                return Result<void, IOError>::make_error(IOError::Other);
        }

        off_t lseek_result = lseek(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, bytes, whence);
        if(lseek_result < 0) {
            return Result<void, IOError>::make_error(IOError::Other); 
        }

        return Result<void, IOError>::make_ok();
    }
    Result<FileMetadata, IOError> C_File::metadata() {
        FileMetadata metadata;
        
        struct stat st;
        if(fstat(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, &st) < 0) {
            return Result<FileMetadata, IOError>::make_error(IOError::Other);
        }

        metadata.size = st.st_size;
        
        return Result<FileMetadata, IOError>::make_ok(metadata);
    }

    void C_File::destroy() {
        int close_result = close(reinterpret_cast<const InternalFileHandle*>(handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a file"));
        }
        deallocate(handle);
    }

    
}

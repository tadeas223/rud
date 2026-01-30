#include "rud/memory.hpp"
#include "rud/os/io.hpp"
#include "rud/result.hpp"
#include "rud/string.hpp"
#include "rud/system.hpp"
#include "rud/variant.hpp"

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace rud::os;
using namespace rud;

constexpr u32 new_file_permissions = 0644;

namespace {
    int file_access_mode_to_flags(FileAccessMode mode) {
        int flags = 0;
        if(mode & FileAccessMode::Read && mode & FileAccessMode::Write) {
            flags |= O_RDWR;
        } else if(mode & FileAccessMode::Read) {
            flags |= O_RDONLY; 
        } else if(mode & FileAccessMode::Write) {
            flags |= O_WRONLY;
        } else {
            return -1; 
        }

        if(mode & FileAccessMode::Append) {
            flags |= O_APPEND; 
        }

        return flags;
    }
    
    int file_make_mode_to_flags(FileCreateMode mode) {
        int flags = 0;
        if(mode & FileCreateMode::Truncate) {
            flags |= O_TRUNC; 
        }
        
        if(mode & FileCreateMode::Create) {
            flags |= O_CREAT; 
        }
        
        return flags;
    }
}

namespace rud::os {
    struct InternalFileHandle {
        int descriptor;
    };

    Result<File, IOError> File::make(const String& path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<File, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>();
        handle->descriptor = open_result;
        return Result<File, IOError>::make_ok(File {.handle = handle});
    }
    
    Result<File, IOError> File::make(const String& path, FileAccessMode access_mode, FileCreateMode make_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if(access_flags < 0) { 
            return Result<File, IOError>::make_error(IOError::InvalidInput);
        }

        int make_flags = file_make_mode_to_flags(make_mode);

        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags|make_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<File, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>();
        handle->descriptor = open_result;
        return Result<File, IOError>::make_ok(File {.handle = handle});
    }
    
    Result<u64, IOError> File::read(void* buffer, u64 size) {
        ssize_t read_result = ::read(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(read_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(read_result);
    }

    Result<u64, IOError> File::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
    
    Result<AllocString, IOError> File::read_to_string() {
        Result<FileMetadata, IOError> r_meta  = metadata();
        if(!r_meta.is_ok()) {
            return Result<AllocString, IOError>::make_error(r_meta.unwrap_error());
        }
        FileMetadata meta = r_meta.unwrap();
        
        ascii* buffer = static_cast<ascii*>(allocate(meta.size));
        
        Result<u64, IOError> r_read = read(buffer, meta.size);
        if(!r_read.is_ok()){
            return Result<AllocString, IOError>::make_error(r_read.unwrap_error());
        }
        
        AllocString string = AllocString::make_take(buffer, meta.size);
        return Result<AllocString, IOError>::make_ok(string);
    }

    Result<u64, IOError> File::write(const String& str) {
        return write(str.chars, str.len);
    }
    
    Result<void, IOError> File::seek(SeekFrom from, u64 bytes) {
        int whence;
        switch(from) {
            case SeekFrom::Current:
                whence = SEEK_CUR;
                break;
            case SeekFrom::Start:
                whence = SEEK_SET;
                break;
            case SeekFrom::End:
                whence = SEEK_END;
                break;
        }

        off_t lseek_result = lseek(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, bytes, whence);
        if(lseek_result < 0) {
            return Result<void, IOError>::make_error(IOError::Other); 
        }

        return Result<void, IOError>::make_ok();
    }

    void File::destroy() const {
        int close_result = close(reinterpret_cast<InternalFileHandle*>(handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a file"));
        }
        deallocate(handle);
    }
    
    Result<FileMetadata, IOError> File::metadata() {
        FileMetadata metadata;
        
        struct stat st;
        if(fstat(reinterpret_cast<InternalFileHandle*>(handle)->descriptor, &st) < 0) {
            return Result<FileMetadata, IOError>::make_error(IOError::Other);
        }

        metadata.size = st.st_size;
        
        return Result<FileMetadata, IOError>::make_ok(metadata);
    }

    StdOut StdOut::make() {
        return {};          
    }

    Result<u64, IOError> StdOut::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(1, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> StdOut::write(const String& str) {
        return write(str.chars, str.len); 
    }

    StdErr StdErr::make() {
        return {};          
    }

    Result<u64, IOError> StdErr::write(const void* buffer, u64 size) {
        ssize_t write_result = ::write(2, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> StdErr::write(const String& str) {
        return write(str.chars, str.len); 
    }

}

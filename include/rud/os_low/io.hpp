#ifndef RUD_OS_LOW_IO_HPP
#define RUD_OS_LOW_IO_HPP

#include "rud/base/result.hpp"
#include "rud/base/string.hpp"
#include "rud/base/types.hpp"
#include "rud/os_low/io_error.hpp"

namespace rud::os_low {
    enum class FileAccessMode : u32 {
        Read = 0x01, Write = 0x02, Append = 0x04, 
    };

    inline FileAccessMode operator|(FileAccessMode a, FileAccessMode b) {
        return static_cast<FileAccessMode>(static_cast<u32>(a) | static_cast<u32>(b)); 
    }
    
    inline bool operator*(FileAccessMode a, FileAccessMode b) {
        return (static_cast<u32>(a) * static_cast<u32>(b)) != 0; 
    }
    
    enum class FileCreateMode : u32 {
        Create = 0x01,
        Truncate = 0x2,
    };

    inline FileCreateMode operator|(FileCreateMode a, FileCreateMode b) {
        return static_cast<FileCreateMode>(static_cast<u32>(a) | static_cast<u32>(b)); 
    }
    
    inline bool operator*(FileCreateMode a, FileCreateMode b) {
        return (static_cast<u32>(a) * static_cast<u32>(b)) != 0; 
    }
    
    struct FileMetadata {
        u64 size;
    };
    
    enum class FileSeekFrom {
        Start,
        End,
        Current
    };

    using FileHandle = void*;
    

    Result<FileHandle, IOError> file_handle_make(const String* path, FileAccessMode access_mode, FileCreateMode create_mode);
    Result<FileHandle, IOError> file_handle_make(const String* path, FileAccessMode access_mode);
        
    Result<u64, IOError> file_handle_read(FileHandle* handle, void* buffer, u64 size);
    Result<u64, IOError> file_handle_write(FileHandle* handle, const void* buffer, u64 size);
        
    Result<void, IOError> file_handle_seek(FileHandle* handle, FileSeekFrom from, u64 bytes); 
    Result<FileMetadata, IOError> file_handle_metadata(FileHandle* handle);

    Result<void, IOError> file_handle_destroy(FileHandle* handle);
    
    using StdStreamHandle = void*;
   
    extern StdStreamHandle stderr_handle;
    extern StdStreamHandle stdout_handle;
    extern StdStreamHandle stdin_handle;

    Result<u64, IOError> std_stream_handle_read(StdStreamHandle* handle, void* buffer, u64 size);
    Result<u64, IOError> std_stream_handle_write(StdStreamHandle* handle, const void* buffer, u64 size);
}

#endif

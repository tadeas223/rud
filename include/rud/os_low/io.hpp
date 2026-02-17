#ifndef RUD_OS_LOW_IO_HPP
#define RUD_OS_LOW_IO_HPP

#include "rud/base/result.hpp"
#include "rud/base/string.hpp"
#include "rud/base/types.hpp"
#include "rud/os_low/io_error.hpp"
#include "rud/ds/c_vector.hpp"

namespace rud::os_low {
    using C_DirectoryHandle = void*;
    
    enum class DirEntryType {
        Directory,
        File
    };

    struct C_DirEntry {
        C_StringAlloc name;
        DirEntryType type;
        
        inline void destroy() {
            name.destroy();
        }
    };

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
    
    Result<C_DirectoryHandle, IOError> c_directory_handle_make(StringView path);
    Result<C_DirectoryHandle, IOError> c_directory_handle_make(StringView path, DirectoryCreateMode create_mode);

    Result<ds::C_DArray<C_DirEntry>, IOError> c_directory_handle_get_entries(C_DirectoryHandle* handle, u32 entry_count);

    void c_directory_handle_destroy(C_DirectoryHandle* handle);

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

    using C_FileHandle = void*;
    
    Result<C_FileHandle, IOError> c_file_handle_make(StringView path, FileAccessMode access_mode, FileCreateMode create_mode);
    Result<C_FileHandle, IOError> c_file_handle_make(StringView path, FileAccessMode access_mode);
    
    Result<C_FileHandle, IOError> c_file_handle_make(const C_DirectoryHandle* dir, StringView path, FileAccessMode access_mode, FileCreateMode create_mode);
    Result<C_FileHandle, IOError> c_file_handle_make(const C_DirectoryHandle* dir, StringView path, FileAccessMode access_mode);
    
        
    Result<u64, IOError> c_file_handle_read(C_FileHandle* handle, void* buffer, u64 size);
    Result<u64, IOError> c_file_handle_write(C_FileHandle* handle, const void* buffer, u64 size);
        
    Result<void, IOError> c_file_handle_seek(C_FileHandle* handle, FileSeekFrom from, u64 bytes); 
    Result<FileMetadata, IOError> c_file_handle_metadata(C_FileHandle* handle);

    void c_file_handle_destroy(C_FileHandle* handle);
    
    using StdStreamHandle = void*;
   
    extern StdStreamHandle std_err_handle;
    extern StdStreamHandle std_out_handle;
    extern StdStreamHandle std_in_handle;

    Result<u64, IOError> std_stream_handle_read(StdStreamHandle* handle, void* buffer, u64 size);
    Result<u64, IOError> std_stream_handle_write(StdStreamHandle* handle, const void* buffer, u64 size);

    Result<void, IOError> directory_handle_set_current_directory(C_DirectoryHandle* handle); 
    Result<C_DirectoryHandle, IOError> directory_handle_get_current_directory(); 
}

#endif

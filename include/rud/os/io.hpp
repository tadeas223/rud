#ifndef RUD_OS_IO_HPP
#define RUD_OS_IO_HPP

#include "rud/result.hpp"
#include "rud/string.hpp"
#include "rud/types.hpp"

namespace rud::os {
    using FileHandle = void*;
    using DirectoryHandle = void*;

    enum class IOError {
        NotFound,
        PermissionDenied,
        AlreadyExists,
        InvalidInput,
        InvalidData,
        Interrupted,
        Other,
        WouldBlock,
        OutOfDiskSpace,
        IvalidFileMode
    };

    enum class FileAccessMode : u32 {
        Read = 0x01,
        Write = 0x02,
        Append = 0x04, 
    };

    enum class SeekFrom {
        Start,
        End,
        Current
    };

    struct FileMetadata {
        u64 size;
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

//    struct Directory {
//        DirectoryHandle handle;
//
//        static Result<Directory, IOError> make(const String path, DirectoryAccessMode access_mode);
//        static Result<Directory, IOError> make(const String path, DirectoryAccessMode access_mode, DirectoryCreateMode create_mode);
//        Result<String, IOError> ls();
//    };

    struct File {
        FileHandle handle;

        static Result<File, IOError> make(const String path, FileAccessMode access_mode);
        static Result<File, IOError> make(const String path, FileAccessMode access_mode, FileCreateMode create_mode);
        
        Result<u64, IOError> read(void* buffer, u64 size);
        
        Result<AllocString, IOError> read_to_string();
        
        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
        
        Result<void, IOError> seek(SeekFrom from, u64 bytes); 
        Result<FileMetadata, IOError> metadata();

        void destroy() const;
    };
    
    struct StdOut {
        static StdOut make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };
    
    struct StdErr {
        static StdErr make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String str);
    };

    inline void debug_print(const String str) {
        StdOut out = StdOut::make();
        out.write(str).except(Lit("stdout closed"));
    }
}

#endif

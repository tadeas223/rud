#ifndef RUD_OS_IO_HPP
#define RUD_OS_IO_HPP

#include "rud/result.hpp"
#include "rud/string.hpp"
#include "rud/types.hpp"

namespace rud::os {
    using FileHandle = void*;

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
    
    template<typename Derived>
    struct Writer {
        Result<u64, IOError> write(const void* buffer, u64 size)
        {
            return static_cast<Derived*>(this)->write(buffer, size);
        }
        
        Result<u64, IOError> write(const String* str) {
            return static_cast<Derived*>(this)->write(str);
        }
    };
    
    template<typename Derived>
    struct Reader {
        Result<u64, IOError> read(const void* buffer, u64 size)
        {
            return static_cast<Derived*>(this)->read(buffer, size);
        }
        
        Result<AllocString, IOError> read_all_to_string() {
            return static_cast<Derived*>(this)->read_all_to_string();
        }
        
        Result<AllocString, IOError> read_line() {
            return static_cast<Derived*>(this)->read_line();
        }
    };
    
    struct File: public Writer<File> {
        FileHandle handle;

        static Result<File, IOError> make(const String* path, FileAccessMode access_mode);
        static Result<File, IOError> make(const String* path, FileAccessMode access_mode, FileCreateMode create_mode);
        
        Result<u64, IOError> read(void* buffer, u64 size);
        
        Result<AllocString, IOError> read_to_string();
        
        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String* str);
        
        Result<void, IOError> seek(SeekFrom from, u64 bytes); 
        Result<FileMetadata, IOError> metadata();

        void destroy() const;
    };
    
    struct StdOut: public Writer<StdOut> {
        static StdOut make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String* str);
    };
    
    struct StdErr: public Writer<StdErr> {
        static StdErr make();

        Result<u64, IOError> write(const void* buffer, u64 size);
        Result<u64, IOError> write(const String* str);
    };

    inline void debug_print(const String* str) {
        StdOut out = StdOut::make();
        out.write(str).except(Lit("stdout closed").temp());
    }
}

#endif

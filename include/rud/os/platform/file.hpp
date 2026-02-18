#ifndef RUD_OS_PLATFORM_FILE_HPP
#define RUD_OS_PLATFORM_FILE_HPP

#include "rud/os/platform/directory.hpp"
#include "rud/os/core/read_stream.hpp"
#include "rud/os/core/write_stream.hpp"
#include "rud/os/core/file_access_mode.hpp"
#include "rud/os/core/file_create_mode.hpp"
#include "rud/os/core/file_seek_from.hpp"
#include "rud/os/core/file_metadata.hpp"
#include "rud/base/result.hpp"

namespace rud::os {
    struct C_File : ReadStream<C_File, IOError>, WriteStream<C_File, IOError> {
        void* handle;

        static Result<C_File, IOError> make(StringView path, FileAccessMode access_mode);
        static Result<C_File, IOError> make(StringView path, FileAccessMode access_mode, FileCreateMode create_mode);
        
        static Result<C_File, IOError> make(C_Directory* dir, StringView path, FileAccessMode access_mode);
        static Result<C_File, IOError> make(C_Directory* dir, StringView path, FileAccessMode access_mode, FileCreateMode create_mode);
        
        Result<u64, IOError> read(void* buffer, u64 size);
        
        Result<u64, IOError> write(const void* buffer, u64 size);
        
        Result<void, IOError> seek(FileSeekFrom from, u64 bytes); 
        Result<FileMetadata, IOError> metadata();

        void destroy();
    };
}

#endif

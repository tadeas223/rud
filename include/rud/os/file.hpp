#ifndef RUD_OS_FILE_HPP
#define RUD_OS_FILE_HPP

#include "rud/os/read_stream.hpp"
#include "rud/os/write_stream.hpp"
#include "rud/os_low/io.hpp"
#include "rud/base/result.hpp"
namespace rud::os {
    struct File : ReadStream<File, os_low::IOError>, WriteStream<File, os_low::IOError> {
        os_low::FileHandle p_handle;

        static Result<File, os_low::IOError> make(const String path, os_low::FileAccessMode access_mode);
        static Result<File, os_low::IOError> make(const String path, os_low::FileAccessMode access_mode, os_low::FileCreateMode create_mode);
        
        Result<u64, os_low::IOError> read(void* buffer, u64 size);
        
        Result<u64, os_low::IOError> write(const void* buffer, u64 size);
        
        Result<void, os_low::IOError> seek(os_low::FileSeekFrom from, u64 bytes); 
        Result<os_low::FileMetadata, os_low::IOError> metadata();

        void destroy();
    };
}

#endif

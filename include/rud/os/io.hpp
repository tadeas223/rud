#ifndef RUD_OS_IO_HPP
#define RUD_OS_IO_HPP

#include "rud/os/io_error.hpp"
#include "rud/result.hpp"
#include "rud/string.hpp"
#include "rud/types.hpp"

namespace rud::os {
//    struct Directory {
//        DirectoryHandle handle;
//
//        static Result<Directory, IOError> make(const String path, DirectoryAccessMode access_mode);
//        static Result<Directory, IOError> make(const String path, DirectoryAccessMode access_mode, DirectoryCreateMode create_mode);
//        Result<String, IOError> ls();
//    };
    
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

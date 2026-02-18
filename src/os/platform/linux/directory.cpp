#include "rud/os/platform/directory.hpp"
#include "rud/os/core/io_error.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

using namespace rud;
using namespace rud::os;
using namespace rud::ds;

namespace rud::os {
    struct InternalFileHandle {
        int descriptor;
    };
    
    struct linux_dirent64 {
        ino64_t        d_ino;
        off64_t        d_off;
        unsigned short d_reclen;
        unsigned char  d_type;
        char           d_name[];
    };

    Result<C_Directory, IOError> C_Directory::make(StringView path) {
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, O_DIRECTORY);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_Directory, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_Directory, IOError>::make_ok({handle});
    }

    Result<C_Directory, IOError> C_Directory::make(StringView path, DirectoryCreateMode create_mode) {
        ascii* cstr = path.to_cstr();
        int open_result = -1;
        
        if(create_mode == DirectoryCreateMode::CreateIfDoesntExist) {
            int fd = open(cstr, O_RDONLY | O_DIRECTORY);
            if (fd >= 0) {
                open_result = fd;
            } else {
                create_mode = DirectoryCreateMode::Create;
            }
        }

        if(create_mode == DirectoryCreateMode::Create) {
            if (mkdir(cstr, 0755) < 0) {
                deallocate(cstr);
                return Result<C_Directory, IOError>::make_error(IOError::Other);
            }
        }


        if(open_result == -1) {
            open_result = open(cstr, O_DIRECTORY);
        }
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_Directory, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_Directory, IOError>::make_ok({handle});
    }
    
    Result<ds::C_DArray<C_DirEntry>, IOError> C_Directory::get_entries(u32 entry_count) {
        C_DArray<C_DirEntry> vec = C_DArray<C_DirEntry>::make();
        char buf[1024];
        
        while(true) {
            int nread = syscall(SYS_getdents64,
                reinterpret_cast<InternalFileHandle*>(handle)->descriptor,
                buf,
                sizeof(buf)
            );
            
            if(nread < 0) {
                return Result<C_DArray<C_DirEntry>, IOError>::make_error(IOError::Other);
            }
            
            if(nread == 0) {
                break;
            }
            
            for(int bpos = 0; bpos < nread;) {
                struct linux_dirent64 *d = (struct linux_dirent64 *)(buf + bpos);
                
                C_DirEntry entry = {
                    .p_name = C_StringAlloc::make_copy_cstr(d->d_name),
                    .type = (d->d_type == DT_REG)? DirEntryType::File : DirEntryType::Directory
                };

                vec.push(entry);

                bpos += d->d_reclen;
            }
        }

        return Result<C_DArray<C_DirEntry>, IOError>::make_ok(vec);
    }

    void C_Directory::destroy() {
        int close_result = close(reinterpret_cast<const InternalFileHandle*>(handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a directory"));
        }
        deallocate(handle);
    }
    
    Result<void, IOError> C_Directory::set_current_directory(C_Directory* directory) {
        int result = fchdir(reinterpret_cast<InternalFileHandle*>(directory->handle)->descriptor);
        if(result < 0) {
            return Result<void, IOError>::make_error(IOError::Other);
        }

        return Result<void, IOError>::make_ok();
    }
    
    Result<void, IOError> C_Directory::set_current_directory(StringView path) {
        Result<C_Directory, IOError> r_dir = C_Directory::make(path);
        if(!r_dir.ok) {
            return Result<void, IOError>::make_error(r_dir.unwrap_error());
        }
        C_Directory dir = r_dir.unwrap();

        return C_Directory::set_current_directory(&dir);
    }
        
    Result<C_Directory, IOError> C_Directory::get_current_directory() {
        return C_Directory::make(Lit("."));
    }
}

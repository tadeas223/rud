#include "rud/os_low/io.hpp"
#include <sys/types.h>

#include "rud/base/memory.hpp"
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>

using namespace rud;
using namespace rud::os_low;
using namespace rud::ds;

namespace {
    struct linux_dirent64 {
        ino64_t        d_ino;
        off64_t        d_off;
        unsigned short d_reclen;
        unsigned char  d_type;
        char           d_name[];
    };

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

namespace rud::os_low {
    struct InternalFileHandle {
        int descriptor;
    };

    Result<C_FileHandle, IOError> c_file_handle_make(StringView path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_FileHandle, IOError>::make_ok(handle);
    }
    
    Result<C_FileHandle, IOError> c_file_handle_make(const C_DirectoryHandle* dir, StringView path, FileAccessMode access_mode, FileCreateMode create_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::InvalidInput);
        }
        
        int create_flags = file_create_mode_to_flags(create_mode);
        
        ascii* cstr = path.to_cstr();
        int open_result = openat(reinterpret_cast<InternalFileHandle*>(*dir)->descriptor, cstr, access_flags|create_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_FileHandle, IOError>::make_ok(handle);

    }
    
    Result<C_FileHandle, IOError> c_file_handle_make(const C_DirectoryHandle* dir, StringView path, FileAccessMode access_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if (access_flags < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::InvalidInput);
        }
        
        ascii* cstr = path.to_cstr();
        int open_result = openat(reinterpret_cast<InternalFileHandle*>(*dir)->descriptor, cstr, access_flags);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_FileHandle, IOError>::make_ok(handle);

    }
    
    Result<C_FileHandle, IOError> c_file_handle_make(StringView path, FileAccessMode access_mode, FileCreateMode create_mode) {
        int access_flags = file_access_mode_to_flags(access_mode);
        if(access_flags < 0) { 
            return Result<C_FileHandle, IOError>::make_error(IOError::InvalidInput);
        }

        int create_flags = file_create_mode_to_flags(create_mode);

        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, access_flags|create_flags, new_file_permissions);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_FileHandle, IOError>::make_ok(handle);
    }
    
    Result<u64, IOError> c_file_handle_read(C_FileHandle* handle, void* buffer, u64 size) {
        ssize_t read_result = ::read(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, buffer, size);
        if(read_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(read_result);
    }

    Result<u64, IOError> c_file_handle_write(C_FileHandle* handle, const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
    
    Result<void, IOError> c_file_handle_seek(C_FileHandle* handle, FileSeekFrom from, u64 bytes) {
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

        off_t lseek_result = lseek(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, bytes, whence);
        if(lseek_result < 0) {
            return Result<void, IOError>::make_error(IOError::Other); 
        }

        return Result<void, IOError>::make_ok();
    }

    void c_file_handle_destroy(C_FileHandle* handle) {
        int close_result = close(reinterpret_cast<const InternalFileHandle*>(*handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a file"));
        }
        deallocate(*handle);
    }
    
    Result<FileMetadata, IOError> c_file_handle_metadata(C_FileHandle* handle) {
        FileMetadata metadata;
        
        struct stat st;
        if(fstat(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, &st) < 0) {
            return Result<FileMetadata, IOError>::make_error(IOError::Other);
        }

        metadata.size = st.st_size;
        
        return Result<FileMetadata, IOError>::make_ok(metadata);
    }
    
    InternalFileHandle internal_std_in = {
        .descriptor = 0
    };

    InternalFileHandle internal_std_out = {
        .descriptor = 1
    };

    InternalFileHandle internal_std_err = {
        .descriptor = 2
    };

    StdStreamHandle std_in_handle = &internal_std_in;
    StdStreamHandle std_out_handle = &internal_std_out;
    StdStreamHandle std_err_handle = &internal_std_err;

    Result<u64, IOError> std_stream_handle_read(StdStreamHandle* handle, void* buffer, u64 size) {
        ssize_t write_result = ::read(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }

    Result<u64, IOError> std_stream_handle_write(StdStreamHandle* handle, const void* buffer, u64 size) {
        ssize_t write_result = ::write(reinterpret_cast<InternalFileHandle*>(*handle)->descriptor, buffer, size);
        if(write_result < 0) {
            return Result<u64, IOError>::make_error(IOError::Other);
        }

        return Result<u64, IOError>::make_ok(write_result);
    }
    
    Result<C_DirectoryHandle, IOError> c_directory_handle_make(StringView path) {
        ascii* cstr = path.to_cstr();
        int open_result = open(cstr, O_DIRECTORY);
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_DirectoryHandle, IOError>::make_ok(handle);
    }

    Result<C_DirectoryHandle, IOError> c_directory_handle_make(StringView path, DirectoryCreateMode create_mode) {
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
                return Result<C_DirectoryHandle, IOError>::make_error(IOError::Other);
            }
        }


        if(open_result == -1) {
            open_result = open(cstr, O_DIRECTORY);
        }
        deallocate(cstr);
        
        if(open_result < 0) {
            return Result<C_FileHandle, IOError>::make_error(IOError::Other); 
        }
        
        InternalFileHandle* handle = allocate<InternalFileHandle>({open_result});
        return Result<C_DirectoryHandle, IOError>::make_ok(handle);
    }

    Result<ds::C_DArray<C_DirEntry>, IOError> c_directory_handle_get_entries(C_DirectoryHandle* handle, u32 entry_count) {
        C_DArray<C_DirEntry> vec = C_DArray<C_DirEntry>::make();
        char buf[1024];
        
        while(true) {
            int nread = syscall(SYS_getdents64,
                reinterpret_cast<InternalFileHandle*>(*handle)->descriptor,
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
                    .name = C_StringAlloc::make_copy_cstr(d->d_name),
                    .type = (d->d_type == DT_REG)? DirEntryType::File : DirEntryType::Directory
                };

                vec.push(entry);

                bpos += d->d_reclen;
            }
        }

        return Result<C_DArray<C_DirEntry>, IOError>::make_ok(vec);
    }

    void c_directory_handle_destroy(C_DirectoryHandle* handle) {
        int close_result = close(reinterpret_cast<const InternalFileHandle*>(*handle)->descriptor);
        if(close_result < 0) {
            panic(Lit("an error occured while closing a directory"));
        }
        deallocate(*handle);
    }
}

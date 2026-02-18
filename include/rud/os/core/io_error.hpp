#ifndef RUD_OS_IO_ERROR_HPP
#define RUD_OS_IO_ERROR_HPP

namespace rud::os {
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
}

#endif

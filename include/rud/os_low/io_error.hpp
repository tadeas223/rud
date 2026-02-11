#ifndef RUD_OS_LOW_IO_ERROR_HPP
#define RUD_OS_LOW_IO_ERROR_HPP

namespace rud::os_low {
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

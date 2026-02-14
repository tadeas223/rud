#ifndef RUD_BASE_ENV_HPP
#define RUD_BASE_ENV_HPP

#if defined (__linux__)
#define OS_LINUX
#endif

#if defined(_WIN_32) || defined(_WIN_64)
#define OS_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define OS_MAC
#endif

#ifdef __cpp_exceptions
#define EXCEPTIONS_ENABLED
#endif

#endif

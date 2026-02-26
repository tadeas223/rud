#ifndef RUD_BASE_ENV_HPP
#define RUD_BASE_ENV_HPP

#if defined(__linux__)
    #define OS_LINUX
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC
#endif


#if defined(__x86_64__) || defined(_M_X64)
    #define CPU_X86_64
#endif

#if defined(__i386__) || defined(_M_IX86)
    #define CPU_X86
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    #define CPU_ARM64
#endif

#if defined(__arm__) || defined(_M_ARM)
    #define CPU_ARM
#endif

#if defined(__riscv)
    #define CPU_RISCV
#endif


#ifdef __cpp_exceptions
    #define EXCEPTIONS_ENABLED
#endif

#endif


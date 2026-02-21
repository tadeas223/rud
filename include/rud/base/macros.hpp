#ifndef RUD_BASE_MACROS_HPP
#define RUD_BASE_MACROS_HPP

#define Concat(a, b) a##b

#define Expand(x) x

#define Stringify(x) Stringify2(x)
#define Stringify2(x) #x
#define LineString Stringify(__LINE__)

#define Kilobytes(x) x * 1024
#define Megabytes(x) x * 1024 * 1024
#define Gigabytes(x) x * 1024 * 1024 * 1024

#endif

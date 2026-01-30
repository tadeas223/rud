#ifndef RUD_MACROS_HPP
#define RUD_MACROS_HPP

#define Concat(a, b) a##b

#define Expand(x) x

#define Stringify(x) Stringify2(x)
#define Stringify2(x) #x
#define LineString Stringify(__LINE__)

#endif

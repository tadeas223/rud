# types
**module:** [root](root.md)

**file header:** `/include/rud/types.hpp`

**no source**

## description
*definitions of basic primitive types*

the current implementation only guarantees that the type has at least the specified size, but the size can be bigger on some platforms.

## contents

all type names contain their sign or type and the amount of bits they take up in memory (`u8` -> unsigned 8 bit integer).

### signed integer types
- `s8`
- `s16`
- `s32`
- `s64`

### unsigned integer types
- `u8`
- `u16`
- `u32`
- `u64`

### floating point number types
- `f32`
- `f64`

### other types
- `ascii`
    
    the ascii type is compatible with the `char` type.
    represents an ascii character, or a c string -> `ascii*`.


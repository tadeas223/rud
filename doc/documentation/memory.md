# memory
**module:** [root](root.md)

**file header:** `/rud/include/memory.hpp`

**file source:** `/src/memory.cpp`

## description
*memory altering / allocating functions*

contains mostly libc like functions for allocating/copying/moving memory.

## contents

### (enum) AllocError
*an error that can occur when memory allocation fails*

| values |
| - |
| OutOfMemory |

---
### (fn) try_allocate_size
`Result<void*, AllocError> try_allocate_size(u64 size)`

tries to allocate memory of the specified size and returns an `AllocError` if it fails

**preconditions:**
- `size != 0`

**error if:**
- the operating system cannot give more memory

**params:**
| param | description |
| - | - |
| size  | number of bytes to allocate |

**returns**:
| success | error |
| - | - |
| pointer to the allocated memory | `AllocError` further specifying why the allocation failed |

---
### (fn) try_allocate
```
template<typename T>
Result<T*, AllocError> try_allocate(T original)
```

allocates `sizeof(T)` bytes and copies the `original` into the allocated memory.

**params:**
| param | description |
| - | - |
| original | initialized value to be copied into the final allocated memory |

**returns**:
| success | error |
| - | - |
| pointer to the initialized allocated memory | `AllocError` further specifying why the allocation failed |

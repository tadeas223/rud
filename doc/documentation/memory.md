# memory
**module:** [root](root.md)

**file header:** `/include/rud/memory.hpp`

**file source:** `/src/memory.cpp`

## description
*memory altering / allocating functions*

contains mostly libc like functions for allocating/copying/moving memory.

## contents

**warning:**
all functions that allocate memory:
 - `allocate_size()`
 - `allocate()`
 - `try_allocate_size()`
 - `try_allocate()`

 must than also call `deallocate()` on the returned pointer, to avoid memory leaks.

---
### (enum) AllocError
*an error that can occur when memory allocation fails*

| values |
| - |
| OutOfMemory |

---
### (fn) try_allocate_size
`Result<void*, AllocError> try_allocate_size(u64 size)`

tries to allocate memory of the specified size and returns an `AllocError` if it fails.

**preconditions:**
| type/result | expected |
| - | - |
| asserted | `size != 0` |

**error if:**
- the operating system cannot give more memory, or an internal error occurred inside the allocator

**params:**
| param | description |
| - | - |
| size  | number of bytes to allocate |

**returns:**
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

### (fn) allocate
```
template<typename T>
inline T* allocate(T original)
```

allocates `sizeof(T)` bytes and copies the `original` into the allocated memory.

**params:**
| param | description |
| - | - |
| original | initialized value to be copied into the final allocated memory |

**returns:**
- a pointer to the allocated memory block

### (fn) deallocate
```
void deallocate(void* ptr)
```

frees the memory chunk pointed to by `ptr`.

**preconditions:**
| type/result | expected |
| - | - |
| undefined behaviour | pointer must be previously allocated with the global allocator | 

**params:**
| param | description |
| - | - |
| ptr | pointer to the memory to be deallocated |

---
### (fn) mem_copy
```
void mem_copy(void* dest, const void* src, u64 size)
```

copies memory from `src` to `dest`.
this function only copies from the first address to the last, it does not check whether pointers are overlapping. for moving memory that overlaps use the safer `mem_move` function.

this function behaves similarly to libc `memcpy`

**preconditions:**
| type/result | expected |
| - | - |
| segmentation fault | `src != nullptr && dest != nullptr` |
| memory corruption | `src + size` and `dest + size` must be accessible for reads and writes |
| memory corruption | `src` and `dest` must not overlap |
| asserted | `size != 0` |

**params:**
| param | description |
| - | - |
| dest | pointer to the memory to copy into |
| src | pointer to the memory to copy from |
| size | number of bytes to copy |

---
### (fn) mem_move
```
void mem_move(void* dest, const void* src, u64 size)
```

moves memory from `src` to `dest`.
this function starts does ensure that memory at `dest` is the same as memory at `src` even if `src` gets overridden.

this function behaves similarly to libc `memmove`

**preconditions:**
| type/result | expected |
| - | - |
| segmentation fault | `src != nullptr && dest != nullptr` |
| memory corruption | `src + size` and `dest + size` must be accessible for reads and writes |
| memory corruption | `src` and `dest` must not overlap |
| asserted | `size != 0` |

**params:**
| param | description |
| - | - |
| dest | pointer to the memory to copy into |
| src | pointer to the memory to copy from |
| size | number of bytes to copy |

---
### (fn) mem_set
```
void mem_set(void* dest, const u8 value, u64 size)
```

this function fills each byte with `value` until `dest + size-1`

this function behaves similarly to libc `memset`

**preconditions:**
| type/result | expected |
| - | - |
| segmentation fault | `dest != nullptr` |
| memory corruption | `src + size` must be accessible for writes |
| asserted | `size != 0` |

**params:**
| param | description |
| - | - |
| dest | memory to write bytes into |
| value | the written byte |
| size | number of bytes to write |

---
### (fn) mem_equals
```
bool mem_equals(const void* ptr1, const void* ptr2, u64 size)
```

checks whether memory at `ptr1` equals the memory at `ptr2` for `size` bytes.

this function behaves similarly to libc `memcmp`

**preconditions:**
| type/result | expected |
| - | - |
| segmentation fault | `ptr1 != nullptr && ptr2 != nullptr` |
| memory corruption | `ptr1 + size` and `ptr2 + size` must be accessible for reads |
| asserted | `size != 0` |

**params:**
| param | description |
| - | - |
| ptr1 and ptr2 | pointers to the memory to compare |
| size | number of bytes to compare |

**returns:**
| true | false |
| - | - |
| if the two memory blocks equal | if the two memory block do not equal |


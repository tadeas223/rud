# string

**module:** [root](root.md)

**file header:** `/include/rud/string.hpp`

**file source:** `/src/memory.cpp`

## description
`length based string types`

contains structures and helper functions for working with length based strings.

## contents

- [String](#(struct)-string)
    - [to_cstr](#(string-member-fn)-to_cstr)
- [AllocString](#(struct)-allocstring)
    - [make_copy_cstr](#(allocstring-make-fn)-make_copy_cstr)
    - [make_copy](#(allocstring-make-fn)-make_copy)
    - [make_copy 2](#(allocstring-make-fn)-make_copy-2)
    - [make_take](#(allocstring-make-fn)-make_take)
    - [destroy](#(allocstring-destroy-fn)-destroy)
- [StringLit]((struct)-stringlit)
    - [make](#(stringlit-make-fn)-make)
    - [make](#(stringlit-make-fn)-make-2)
    - [make_cstr](#(stringlit-make-fn)-make-cstr)
- [cstr_len](#(fn)-cstr_len)

---
### (struct) String
```
struct String {
    ascii* chars;
    u32 len;
}
```

a length based string structure. user should not hold or initialize this struct. it exists for taking both `AllocString` and `StringLit` in a function parameter.

usecase of this struct:
```
void takes_any_string(const String* str) {
    // work with the string
}

int main() {
    StringLit lit = Lit("literal string")
    StringAlloc allocated = StringAlloc::make_copy("allocated string")

    takes_any_string(&lit);
    takes_any_string(&allocated);

    allocated.destroy();
}
```

in this example the `takes_any_string()` function can take any type of string no matter whether its was allocated or not.

---
### (String member fn) to_cstr
```
ascii* to_cstr() const
```

converts the `String` instance into a null-terminated string. the returned pointer and must be deallocated with `deallocate()`.

**returns:**
- a pointer to the allocated null-terminated string

---
### (struct) AllocString
```
struct AllocString : String {}
```

a structure representing a length based string that holds an allocated buffer. this string must be destroyed after use.

---
### (StringAlloc make fn) make_copy
```
static AllocString make_copy(const ascii* chars, u32 len)
```

makes the `AllocString` by copying a buffer pointed to by `chars` with the `len` length. this function creates a whole copy of the specified buffer.

**params:**
| param | description |
| - | - |
| chars | pointer to the string buffer to copy |
| len | number of characters to copy |

**returns:**
- newly created `AllocString`

---
### (AllocString make fn) make_copy 2
```
template<u32 N>
static constexpr AllocString make_copy(const ascii (&s)[N]) {
    return make_copy(s, N-1);
}
```

makes the `AllocString` by copying a string literal. this function gets the length of the string at compile time through a template. do not use this function for string variables or normal pointers, because it wont work.

**params:**
| param | description |
| - | - |
| s | a string literal to copy |

**returns:**
- newly created `AllocString`

---
### (AllocString make rn) make_copy_cstr
```
static AllocString make_copy_cstr(const ascii* cstr)
```

makes the `AllocString` by copying the null-terminated string pointed to by `cstr`. this function determies the length of the string by counting from the string start until `\0` and this is very slow, use the `make_copy` function instead of this one if the length is known.

**params:**
| param | description |
| - | - |
| cstr | null-terminated string to copy |

**returns:**
- newly created `AllocString`

---
### (AllocString make fn) make_take
```
static AllocString make_take(const ascii* chars, u32 len)
```

makes the `AllocString` by taking the specified buffer. this function does not copy the buffer but it takes ownership of it. when the `AllocString` is destroyed the buffer will be deallocated.

**params:**
| param | description |
| - | - |
| chars | string buffer to take ownership of |
| len | number of characters |

**returns:**
- newly created `AllocString`

---
### (AllocString destroy fn) destroy
```
void destroy() const
```

deallocates the `AllocString` buffer.

---
### (struct) StringLit

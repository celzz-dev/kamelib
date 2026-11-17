# kamelib

A header-only C standard library replacement built from scratch for Linux, with the long-term goal of running without `glibc`.

> Note: This project is an active work in progress.

## Modules

| Header | Description | Key Functions / Types |
| :--- | :--- | :--- |
| `kamelib.h` | Umbrella header that includes all `kamelib` modules. | — |
| `kmem.h` | Custom heap allocator built on `mmap` with block splitting and coalescing. | `_malloc`, `_realloc`, `_free` |
| `kio.h` | Formatted output built directly over the Linux `write` syscall. | `_print`, `_printin` |
| `kproc.h` | Process memory reader, writer, and scanner using `/proc/<pid>/mem` (inspired by Cheat Engine and PINCE). | `proc_getpid`, `proc_readmem`, `proc_writemem`, `proc_scanmem`, `proc_rescanmem` |
| `kstr.h` | Dynamic string implementation similar to C++ `std::string`. | Custom string struct & helpers |
| `kvec.h` | Dynamic vector implementations for strings and memory addresses. | String vector, `alist` (address list) |
| `ktypes.h` | Core type definitions. | `usize`, base types |

## Quick Start

`kamelib` is header-only. Copy the `.h` files into your project and include `kamelib.h` (or only the specific headers you need):

```c
#include "kamelib.h"

int main(void) {
    int *nums = (int *)_malloc(sizeof(int) * 4);
    nums[0] = 42;

    _print("Value: %d\n", nums[0]);

    _free(nums);
    return 0;
}
```

Compile with GCC:

```bash
gcc main.c -o main
```

## Format Specifiers (`kio.h`)

`_print` (like `printf`) and `_printin` (like `snprintf`) currently support these specifiers:

| Specifier | Type | Supported In | Description |
| :--- | :--- | :--- | :--- |
| `%d` | `int` | `_print`, `_printin` | Signed decimal integer |
| `%c` | `char` | `_print`, `_printin` | Single ASCII character |
| `%cs` | `char *` / `char[]` | `_print`, `_printin` | Null-terminated C string |
| `%s` | `kstr` | `_print`, `_printin` | Custom `kamelib` string |
| `%us` | `usize` | `_print`, `_printin` | Custom unsigned size type |
| `%v` | `kvec` | `_print` only | Custom `kamelib` vector |

## Platform Support

* **Linux:** Supported (required for `kproc.h` and raw syscalls).
* **macOS / Windows:** Not currently supported.

## Roadmap

* [ ] Add input functions to `kio.h`
* [ ] Generalize `kvec.h` to support arbitrary data types beyond strings and `alist`
* [ ] Add more format specifiers to `_print` and `_printin`
* [ ] Add comments across the codebase explaining internal implementation details
* [ ] Full `-nostdlib` compatibility

## License

[MIT](LICENSE)


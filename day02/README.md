# Custom Deleters

## Day Overview

On Day 1 we built a hand-written RAII wrapper around a raw file descriptor,
implementing the Rule of Five to guarantee deterministic cleanup.  That approach
works, but it is boilerplate-heavy and error-prone for every new resource type.

Today we replace the manual wrapper with **`std::unique_ptr<T, Deleter>`** — the
standard library's general-purpose RAII smart pointer — equipped with a
**custom deleter** that encapsulates the cleanup logic.  The result is the same
exception-safe, deterministic release with far less code.

## Learning Objectives

- Understand how `std::unique_ptr<T, Deleter>` accepts a second template
  parameter that controls what happens when the pointer is destroyed.
- Know the three common deleter forms: **function pointer**, **stateless
  functor (struct)**, and **lambda with `decltype`** — and when to prefer each.
- Recognise how the deleter type affects the size and layout of the
  `unique_ptr` object (zero-overhead for stateless functors/captureless
  lambdas; one extra pointer for function pointers).
- Use a type alias (`using FileHandlePtr = …`) to keep call-sites readable.
- Implement a factory function that opens a resource and returns a
  `unique_ptr` with a custom deleter already attached.
- Compare trade-offs between a full hand-written RAII class (Day 1) and a
  `unique_ptr`-with-deleter approach.
- Verify deterministic cleanup through scope exit and ownership transfer via
  `std::move`.

## Task Summary

Refactor the raw-integer file descriptor from Day 1 into a
`std::unique_ptr<int, FileDescriptorDeleter>`.  The deleter must call the
pseudo-close function exactly once for every successfully opened handle.
Ownership must be unique — moving a handle transfers it, and leaving a scope
destroys it.

## Implementation Requirements

| Component | Description |
|---|---|
| `FileDescriptorDeleter` | A stateless functor whose `operator()(int* fd)` closes the descriptor and frees the heap-allocated int. |
| `FileHandlePtr` | A type alias: `std::unique_ptr<int, FileDescriptorDeleter>`. |
| `make_file_handle(name)` | Factory that opens a pseudo file, heap-allocates the descriptor, and returns a `FileHandlePtr`. |
| `is_valid_handle(h)` | Returns `true` when the handle is non-null and the descriptor is non-negative. |
| `get_handle(h)` | Returns the raw descriptor value; throws on invalid handle. |
| `get_close_count()` | Returns the number of times the deleter has invoked `close_pseudo_file` — used by tests to observe cleanup. |

## How to Run

From the repository root:

```bash
# Compile and run the tests
make day=02 test

# Run under AddressSanitizer
make day=02 asan

# Run under Valgrind
make day=02 valgrind

# Remove build artefacts
make day=02 clean
```

## Suggested Workflow

1. Read through `materials/notes.md` to understand how custom deleters work.
2. Study the declarations in `exercise.hpp` — the type alias, deleter struct,
   and factory signature.
3. Implement each function in `exercise.cpp`, starting with the pseudo
   open/close helpers, then the deleter, then the factory and query functions.
4. Run `make day=02 test` and fix any failures.
5. Run the sanitiser / Valgrind targets to confirm no leaks or undefined
   behaviour.
6. Review `materials/links.md` for deeper reading.

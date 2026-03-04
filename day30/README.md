# Small Buffer Optimization

## Day Overview

In Day 29 we built an `Any` class that heap-allocates a `holder<T>` for every
stored value. For programs that create and destroy many small `Any` objects —
integers, pointers, small structs — those frequent heap allocations become a
measurable cost: allocator overhead, cache misses, and memory fragmentation.

**Small Buffer Optimization (SBO)** eliminates heap allocation for small types
by embedding an aligned byte buffer directly inside the wrapper. When the held
type fits within that buffer, the object is constructed in-place with placement
`new`; only objects that exceed the buffer fall back to the heap. Both
`std::any` and `std::function` in major standard-library implementations use
this technique.

Today we refactor the Day 29 `Any` into a `SmallAny` that applies SBO. The
type-erasure pattern (`placeholder` / `holder<T>`) remains, but storage
management becomes significantly more involved.

## Learning Objectives

- Explain the motivation for Small Buffer Optimization: avoiding heap
  allocation for small, frequently-used types.
- Use `alignas` and `alignof` to create properly aligned inline storage.
- Construct objects in raw storage with placement `new`.
- Manually invoke destructors when objects live in inline storage.
- Determine at compile time whether a type fits inline using `sizeof` and
  `alignof` constraints with `if constexpr`.
- Implement correct copy, move, and destruction for both inline and heap paths.
- Understand why `std::any` and `std::function` employ SBO internally.
- Appreciate the added complexity SBO introduces over a simple `unique_ptr`
  approach.

## Task Summary

Implement a class `SmallAny` that stores any copyable type using the
placeholder / holder\<T\> pattern from Day 29, but with an inline buffer for
small types and heap fallback for large ones. The class exposes `has_value()`,
`type()`, `reset()`, and `is_inline()`. Free-function `any_cast` overloads
retrieve the stored value — pointer overloads return `nullptr` on type
mismatch; value overloads throw `std::bad_cast`.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| Buffer size | `static constexpr std::size_t buf_size = 32;` |
| Aligned storage | `alignas(buf_align) unsigned char storage_[buf_size];` |
| `placeholder` (abstract base) | Nested struct with virtual `clone_to()`, `clone_heap()`, `type()`, and destructor. |
| `holder<T>` (concrete model) | Derives from `placeholder`, stores a `T value`. |
| Inline decision | `sizeof(holder<T>) <= buf_size && alignof(holder<T>) <= buf_align` |
| Inline path | Placement `new` into `storage_`; manual destructor call on destroy. |
| Heap path | `new holder<T>(...)` on construction; `delete ptr_` on destroy. |
| Copy semantics | `clone_to()` for inline objects, `clone_heap()` for heap objects. |
| Move semantics | Clone-then-destroy for inline objects, pointer steal for heap objects. |
| `has_value()` | Returns `true` when a value is held. |
| `type()` | Returns `typeid(T)` of held type, or `typeid(void)` when empty. |
| `reset()` | Destroys the stored value. |
| `is_inline()` | Returns whether the current value lives in the inline buffer. |
| `any_cast<T>(SmallAny*)` | Returns `T*` or `nullptr` on type mismatch. |
| `any_cast<T>(SmallAny&)` | Returns `T` by value or throws `std::bad_cast`. |

## How to Run

From the repository root:

```bash
make day=30 test       # build and run tests
make day=30 asan       # AddressSanitizer build
make day=30 valgrind   # Valgrind memcheck
make day=30 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study `exercise.hpp` to understand the placeholder / holder pattern and the
   SBO buffer layout.
3. Trace through the inline and heap construction paths.
4. Trace through the copy and move operations for both paths.
5. Run `make day=30 test` and verify all assertions pass.
6. Run the ASan and Valgrind targets to confirm no leaks or memory errors.
7. Answer the self-check questions in `notes.md` without peeking at references.

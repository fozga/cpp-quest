# Small Buffer Optimization — Notes

## A. What Is Small Buffer Optimization?

Small Buffer Optimization (SBO) is a technique in which a class reserves a
small, fixed-size buffer as part of its own storage. When the object to be
stored is small enough, it is constructed directly inside that buffer using
placement `new`, avoiding a heap allocation entirely. Only objects that exceed
the buffer's capacity fall back to `new` on the heap.

SBO is widely used in production standard-library implementations:

- **`std::string`** — most implementations store short strings (typically up to
  15 or 22 characters) inline inside the `std::string` object itself. This is
  often called the Short String Optimization (SSO).
- **`std::function`** — small callables (e.g. function pointers, small lambdas
  with few captures) are stored inline; larger callables are heap-allocated.
- **`std::any`** — small types are stored in an internal buffer; large types go
  to the heap.

The key benefit is performance: heap allocation involves system calls or
allocator bookkeeping, produces cache-unfriendly pointer chasing, and
contributes to memory fragmentation. Inline storage keeps the data adjacent to
the wrapper, improving locality and eliminating allocator overhead for common
small types like `int`, `double`, pointers, and small structs.

## B. Implementing SBO: Aligned Storage and Placement New

### Aligned storage

The inline buffer must satisfy the alignment requirements of any type we might
place in it. We use `alignas` to enforce alignment:

```cpp
static constexpr std::size_t buf_size  = 32;
static constexpr std::size_t buf_align = alignof(std::max_align_t);
alignas(buf_align) unsigned char storage_[buf_size];
```

`std::max_align_t` has the strictest fundamental alignment on the platform
(typically 16 bytes on x86-64). This ensures any standard type can be placed in
the buffer without violating alignment rules.

### Placement new

Placement `new` constructs an object at a specific memory address without
allocating memory:

```cpp
auto* p = ::new(storage_) holder<int>(42);
```

This calls `holder<int>`'s constructor and places the resulting object at the
address `storage_`. The memory must be suitably aligned and large enough.

### Manual destructor call

Objects constructed with placement `new` must have their destructors called
explicitly — there is no `delete` to pair with placement `new`:

```cpp
ptr_->~placeholder();   // invokes the virtual destructor
```

This is one of the few places in C++ where you legitimately call a destructor
directly. After this call, the memory in `storage_` is available for reuse.

## C. Inline vs Heap Decision

At compile time we can determine whether a given `holder<T>` fits in the
buffer:

```cpp
template <typename T>
static constexpr bool fits_inline =
    (sizeof(holder<T>) <= buf_size) && (alignof(holder<T>) <= buf_align);
```

Using `if constexpr`, the constructor selects the appropriate path:

```cpp
if constexpr (fits_inline<std::decay_t<T>>) {
    // placement new into storage_
} else {
    // ordinary new on the heap
}
```

A `holder<T>` contains a vtable pointer (typically 8 bytes on 64-bit systems)
plus the `T value` member, padded to alignment. For example:

| Type | `sizeof(T)` (typical) | `sizeof(holder<T>)` (typical) | Fits in 32 bytes? |
|---|---|---|---|
| `int` | 4 | 16 | Yes |
| `double` | 8 | 16 | Yes |
| `std::string` | 32 | 40 | No |

Types like `int` and `double` go inline; `std::string` (with its 32-byte SSO
buffer on most implementations) exceeds the limit and goes to the heap.

## D. Responsibilities: Correct Destruction, Copy, and Move

SBO complicates every operation because we must handle two storage paths:

### Destruction

- **Inline:** call the virtual destructor manually (`ptr_->~placeholder()`),
  do **not** `delete`.
- **Heap:** call `delete ptr_` which invokes the destructor and frees memory.

A `bool inline_` flag tracks which path to take.

### Copy

- **Inline source:** call `clone_to()`, which uses placement `new` to
  construct a copy in the destination's buffer.
- **Heap source:** call `clone_heap()`, which returns a `unique_ptr` from
  `make_unique<holder<T>>(value)`, then release ownership to the raw pointer.

### Move

- **Inline source:** we cannot simply copy the raw bytes (the object may have
  internal pointers or non-trivial state). For safety, we clone the inline
  object into the destination's buffer, then destroy the source.
- **Heap source:** steal the pointer — set `ptr_` to the source's pointer
  and null out the source.

## E. Self-Check Questions

1. What problem does Small Buffer Optimization solve compared to always
   heap-allocating?
2. Why must the inline buffer be aligned with `alignas`, and what alignment
   value is typically chosen?
3. What is placement `new` and how does it differ from ordinary `new`?
4. Why must you call the destructor explicitly for objects constructed with
   placement `new`?
5. How does the class decide at compile time whether a type fits inline?
6. Why can't you simply `memcpy` the inline buffer when moving a `SmallAny`
   that holds a non-trivially-moveable type?
7. What is the role of the `clone_to()` virtual function, and how does it
   differ from `clone_heap()`?
8. Why does `holder<std::string>` typically not fit in a 32-byte buffer?
9. If you increase `buf_size` to 64, which types would now be stored inline
   that weren't before? What is the trade-off?
10. How do `std::any` and `std::function` in the standard library use SBO?

# Memory Pooling

## Day Overview

Welcome to **Day 4** of the 40-day C++ quest. Today you tackle **memory
pooling** — a technique that replaces many small, individual heap allocations
with a single pre-allocated block of memory divided into fixed-size slots.

When a program repeatedly allocates and frees small objects (graph nodes, linked
list entries, event packets), the default allocator pays a cost on every call:
metadata bookkeeping, potential lock contention, and growing fragmentation that
scatters hot data across cache lines. A memory pool eliminates most of that
overhead by serving allocations from a contiguous buffer with O(1) pointer
arithmetic.

C++ exposes this idea through the **Allocator** concept. Any standard container
or smart-pointer factory that accepts an allocator parameter can be redirected to
your pool. `std::allocate_shared` is especially useful: it fuses the reference
count control block and the managed object into a single allocation, which pairs
naturally with pooling when you are constructing many `shared_ptr`-owned nodes.

---

## Learning Objectives

By the end of this day you will be able to:

- Explain why many small heap allocations hurt performance (overhead,
  fragmentation, poor cache locality).
- Describe the basic memory pool model: pre-allocate a contiguous block, divide
  it into fixed-size slots, and manage a free list.
- Implement a `SimplePool` class that owns a byte buffer and hands out
  fixed-size slots.
- Write a minimal C++20-conforming `PoolAllocator<T>` that forwards to
  `SimplePool`.
- Use `std::allocate_shared` to construct `shared_ptr` objects whose storage
  comes entirely from the pool.
- Explain how allocator **rebinding** works and why `allocate_shared` requires
  it.
- Connect pool allocation back to RAII and smart-pointer ownership: the pool
  owns the raw memory, smart pointers own the object lifetime.

---

## Task Summary

You will build a high-allocation node system and then pool its memory:

1. Define a trivial `Node` struct holding a single `int`.
2. Implement `SimplePool` — a fixed-capacity arena of equal-sized slots backed
   by a contiguous byte buffer and a free list.
3. Implement `PoolAllocator<T>` — a minimal allocator that satisfies the C++20
   allocator requirements and delegates to a `SimplePool`.
4. Write `make_nodes_with_pool` — a function that constructs a vector of
   `shared_ptr<Node>` using `std::allocate_shared` and your pool allocator.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| `SimplePool` constructor | Takes `capacity` (number of slots) and `slot_size` (bytes per slot); allocates a contiguous buffer and initialises a free list |
| `SimplePool::allocate` | Returns `void*` to the next free slot; throws `std::bad_alloc` when exhausted |
| `SimplePool::deallocate` | Returns a slot to the free list |
| `SimplePool::allocation_count` | Returns the number of currently outstanding allocations |
| `PoolAllocator<T>` | `value_type = T`; constructor takes `SimplePool&`; `allocate`/`deallocate` delegate to the pool |
| Rebind support | `rebind<U>::other` typedef **and** a rebind (converting) constructor so `allocate_shared` can rebind internally |
| Equality operators | Two `PoolAllocator`s are equal iff they point to the same `SimplePool` |
| `make_nodes_with_pool` | Accepts `count` and `SimplePool&`; returns `std::vector<std::shared_ptr<Node>>` with values `0 .. count-1` |
| Slot size | 256 bytes — large enough to hold the shared_ptr control block + `Node` together |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=04 test

# Build with AddressSanitizer and run
make day=04 asan

# Build a normal binary then run under Valgrind
make day=04 valgrind

# Remove compiled artefacts
make day=04 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand memory pools and the C++
   allocator model.
2. **Open** `solution/exercise.hpp` and study the `SimplePool` and
   `PoolAllocator` interfaces.
3. **Implement** `SimplePool` in `solution/exercise.cpp` — start with the
   constructor and the free list, then `allocate`, `deallocate`, and the
   counter.
4. **Implement** `make_nodes_with_pool` using `std::allocate_shared`.
5. **Compile** with `make day=04 test` — fix any compiler errors.
6. **Run** the tests and make every assertion pass.
7. **Run** under ASan (`make day=04 asan`) and Valgrind
   (`make day=04 valgrind`) to confirm zero leaks and no pool corruption.
8. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
9. **Browse** the links in `materials/links.md` for deeper understanding.

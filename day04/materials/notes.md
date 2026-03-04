# Memory Pooling — Study Notes

## A. Motivation: Why Pool Memory?

Every call to `new` (or `malloc`) asks the runtime allocator to find a suitable
chunk of free memory. For large, infrequent allocations this is fine. When a
program allocates and frees thousands of small objects — graph nodes, AST nodes,
network packets, ECS components — three costs compound:

1. **Per-allocation overhead.** The allocator stores bookkeeping metadata (size,
   alignment, free-list pointers) next to every allocation. For a 16-byte
   object, metadata can double the real memory consumed.

2. **Fragmentation.** After many allocate/free cycles, free memory becomes a
   patchwork of small gaps. Even though enough total free memory exists, no
   single contiguous region may be large enough to satisfy the next request.

3. **Cache locality.** Objects allocated at different times land at unrelated
   addresses. Traversing a linked structure causes cache misses on nearly every
   pointer dereference.

A **memory pool** sidesteps all three by pre-allocating a single contiguous
block, dividing it into equal-sized **slots**, and serving each allocation
request by handing out the next free slot. Deallocation returns the slot to a
**free list** without ever touching the system allocator.

---

## B. Basic Memory Pool Model

A minimal pool contains:

- **Buffer** — a contiguous byte array, typically `capacity × slot_size` bytes.
- **Slot size** — large enough to hold the object (plus alignment padding).
- **Free list** — a singly-linked list threaded *through* the unused slots
  themselves. Each free slot's first `sizeof(void*)` bytes store a pointer to
  the next free slot, so the free list needs no extra memory.
- **Allocate** — pop the head of the free list and return its address. O(1).
- **Deallocate** — push the returned slot onto the head of the free list. O(1).

Because every slot is the same size, the pool cannot suffer internal
fragmentation beyond the per-slot padding. Because the buffer is contiguous,
objects allocated from the pool enjoy good spatial locality.

### Free-list initialisation

```
slot 0        slot 1        slot 2      ...   slot N-1
[next=slot1]  [next=slot2]  [next=slot3] ...  [next=nullptr]
 ^
 head
```

Each slot stores a pointer to the next slot. `head` points to slot 0. On
`allocate`, `head` advances to `slot1`. On `deallocate(slot0)`, `slot0.next` is
set to the current head and head rewinds to `slot0`.

---

## C. The C++ Allocator Model

The C++ standard library lets you swap the allocation strategy of any
allocator-aware container (or `allocate_shared`) by providing a type that
satisfies the **Allocator** named requirements.

For C++20, a minimal allocator needs:

| Requirement | Meaning |
|---|---|
| `value_type` | The type of object the allocator constructs |
| `allocate(n)` | Return a pointer to storage for `n` objects of `value_type` |
| `deallocate(p, n)` | Release storage previously obtained from `allocate` |
| `operator==` / `operator!=` | Two allocators compare equal if memory from one can be freed by the other |

Older code may also expect:

- `rebind<U>::other` — an allocator of the same *family* but for type `U`.
  Containers often need to allocate internal node types that differ from the
  user-visible `value_type`. C++20 deprecates explicit `rebind` in favour of
  `std::allocator_traits::rebind_alloc`, but providing it improves portability.

- A **converting (rebind) constructor** — `PoolAllocator<T>` must be
  constructible from `PoolAllocator<U>` so the library can convert between
  rebound types.

---

## D. `std::allocate_shared`

`std::make_shared<T>(args...)` allocates the control block (reference counts,
deleter) and the `T` object in a **single** allocation. This reduces overhead
and improves locality compared to a raw `new T` + `shared_ptr` wrapping.

`std::allocate_shared<T>(alloc, args...)` does the same, but uses *your*
allocator instead of the global `operator new`. The library will **rebind** your
allocator to an internal type (typically something like
`_Sp_counted_ptr_inplace<T, Alloc>`) that is *larger* than `T` because it
includes the reference counts. Your pool's slot size must be large enough to
hold this combined object.

Benefits for large graphs:

- All nodes live in a dense pool → good cache behaviour during traversal.
- Allocation is O(1) per node → predictable latency.
- The pool can be freed in bulk when the graph is torn down.

---

## E. Designing `PoolAllocator<T>`

Our teaching design:

1. **`SimplePool`** — a non-template class that owns the byte buffer, manages
   the free list, and exposes `void* allocate(size_t)` /
   `void deallocate(void*, size_t)`. It knows nothing about types.

2. **`PoolAllocator<T>`** — a lightweight handle that stores a pointer to a
   `SimplePool` and satisfies the allocator requirements. Because it is a
   template, it can be rebound to any `U` while still pointing at the same
   underlying pool.

Limitations versus industrial-strength allocators:

- All slots are the same size, wasting space for mixed-size allocations.
- The pool has a fixed capacity chosen at construction time.
- Thread safety is not addressed (a real pool would need a lock or per-thread
  pools).
- No alignment guarantees beyond `alignof(std::max_align_t)` without extra
  work.
- No support for allocations larger than one slot.

These limitations are acceptable for a teaching exercise. Production code would
use libraries like Boost.Pool, `std::pmr::monotonic_buffer_resource`, or custom
arenas with per-size-class free lists.

---

## F. Self-Check Questions

Answer these without looking at the notes above.

1. What three costs make many small heap allocations inefficient?

2. What is a free list, and why can it be stored *inside* the unused pool slots?

3. What is the time complexity of allocate and deallocate in a free-list pool?

4. In the C++20 allocator model, what minimum set of members must a custom
   allocator provide?

5. Why does `std::allocate_shared` need to **rebind** the allocator you give
   it?

6. How much larger is the rebind target type compared to the `Node` type, and
   why does this matter for the pool's slot size?

7. When two `PoolAllocator` instances compare equal, what does that guarantee
   about memory allocated through either one?

8. What happens if the pool runs out of slots and `allocate` is called?

9. How does a memory pool improve cache locality compared to the default
   allocator?

10. Name two limitations of our `SimplePool` that an industrial-strength
    allocator would address.

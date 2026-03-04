# Relaxed Atomics — Notes

## A. Memory Ordering Overview

C++11 defines six memory orderings in `<atomic>`:

| Ordering | Description |
|---|---|
| `memory_order_relaxed` | Guarantees atomicity only. No ordering constraints on surrounding operations. |
| `memory_order_consume` | Data-dependency ordering (rarely used; most compilers promote to acquire). |
| `memory_order_acquire` | A load with acquire ensures that subsequent reads/writes in the current thread are not reordered before this load. |
| `memory_order_release` | A store with release ensures that preceding reads/writes in the current thread are not reordered after this store. |
| `memory_order_acq_rel` | Combines acquire and release; used for read-modify-write operations that both load and store. |
| `memory_order_seq_cst` | Full sequential consistency. All `seq_cst` operations appear in a single global total order. This is the default. |

## B. Sequential Consistency vs Relaxed

**`memory_order_seq_cst`** (the default when you omit the ordering argument) is
the easiest to reason about: all threads see atomic operations in the same
order. The trade-off is a full memory fence on every operation, which can be
expensive on ARM and POWER architectures.

**`memory_order_relaxed`** guarantees atomicity — no torn reads or writes — but
says nothing about the order in which other operations become visible. It is
safe when the atomic variable's value is independent of all other shared state
(e.g., a standalone counter, a statistics accumulator).

## C. Release / Acquire Pattern

The release/acquire pair creates a **happens-before** relationship:

1. **Producer** performs non-atomic writes, then a `store(release)`.
2. **Consumer** performs a `load(acquire)` on the same atomic variable.
3. If the consumer's load reads the value written by the producer's store, then
   *all* writes that happened before the store are guaranteed to be visible to
   the consumer after the load.

This is the fundamental pattern for passing data between threads without a
mutex. Example:

```cpp
// Producer
data = 42;                                      // non-atomic write
flag.store(true, std::memory_order_release);    // release store

// Consumer
if (flag.load(std::memory_order_acquire)) {     // acquire load
    assert(data == 42);                         // guaranteed to see 42
}
```

The key insight: `release` prevents preceding writes from being reordered past
the store, and `acquire` prevents subsequent reads from being reordered before
the load. Together, they synchronize the producer's writes with the consumer's
reads.

## D. Self-Check Questions

1. How many memory orderings does C++ define? Name all six.
2. What does `memory_order_relaxed` guarantee, and what does it *not* guarantee?
3. Why is `memory_order_seq_cst` the default for `std::atomic` operations?
4. What is a "happens-before" relationship and why does it matter?
5. In the release/acquire pattern, which side uses release and which uses acquire?
6. If a producer stores with `memory_order_release`, can a consumer safely read with `memory_order_relaxed`? Why or why not?
7. When is `memory_order_relaxed` sufficient and safe to use?
8. What does `memory_order_acq_rel` do, and when would you use it?
9. Why is `memory_order_consume` rarely used in practice?
10. On x86, `memory_order_seq_cst` stores emit an `MFENCE` or `XCHG`. Why is this not needed for acquire/release on x86?

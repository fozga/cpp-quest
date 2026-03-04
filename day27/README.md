# Lock-Free Basics

## Day Overview

Mutexes serialize access and guarantee correctness, but they also block waiting
threads. **Lock-free** programming uses `std::atomic` operations — particularly
**compare-and-swap (CAS)** loops — to update shared state without ever holding a
lock. A lock-free algorithm guarantees that *at least one* thread makes progress
in a finite number of steps, even if other threads stall.

This day introduces `std::atomic`, its core operations (`load`, `store`,
`compare_exchange_weak/strong`), and the CAS retry loop pattern. Day 28 builds
on this foundation by exploring memory orderings.

## Learning Objectives

- Explain why plain variables are insufficient for concurrent access and how `std::atomic` provides indivisible operations.
- Distinguish between read-modify-write operations (`fetch_add`, `exchange`) and CAS-based approaches.
- Implement a thread-safe counter using a CAS loop with `compare_exchange_weak`.
- Describe the difference between `compare_exchange_weak` and `compare_exchange_strong`.
- Understand lock-free progress guarantees: lock-free vs wait-free vs obstruction-free.

## Task Summary

Implement a `LockFreeCounter` class:

1. `increment()` — atomically increments the counter by 1 using a CAS loop.
2. `add(int n)` — atomically adds an arbitrary integer using a CAS loop.
3. `value()` — returns the current counter value.
4. A multi-threaded stress test spawns several threads each calling `increment()` thousands of times, then asserts the final count is correct.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `LockFreeCounter` class | Contains a `std::atomic<int> counter_` initialised to 0. |
| `increment()` | Uses a `compare_exchange_weak` CAS loop to add 1. |
| `add(int n)` | Uses a `compare_exchange_weak` CAS loop to add `n`. |
| `value()` | Returns `counter_.load()`. Marked `const noexcept`. |
| Thread safety | No mutexes; correctness relies entirely on atomic CAS. |

## How to Run

From the repository root:

```bash
make day=27 test       # build and run tests
make day=27 asan       # AddressSanitizer build
make day=27 valgrind   # Valgrind memcheck
make day=27 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Implement `increment()`, `add()`, and `value()` in `exercise.cpp`.
4. Run `make day=27 test` and iterate until all assertions pass.
5. Run the ASan and Valgrind targets to confirm no data races or leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.

# Relaxed Atomics

## Day Overview

Every `std::atomic` operation accepts a **memory ordering** argument that
controls how surrounding reads and writes may be reordered relative to the
atomic operation. The default is `memory_order_seq_cst` (sequentially
consistent), which provides the strongest guarantees but may cost performance on
weakly-ordered architectures. Weaker orderings — `relaxed`, `acquire`,
`release`, and `acq_rel` — give the programmer fine-grained control over
visibility and ordering, enabling faster lock-free data structures when the full
strength of sequential consistency is not needed.

This day builds on the lock-free CAS techniques from Day 27 and focuses on
choosing the right memory ordering for the job.

## Learning Objectives

- List all six `std::memory_order` values and state what each guarantees.
- Explain the difference between `memory_order_seq_cst` and `memory_order_relaxed`.
- Apply the **release/acquire** pattern to publish data from a producer to a consumer without a mutex.
- Implement a `ReadyFlag` class using release-store and acquire-load.
- Implement a `RelaxedCounter` using `fetch_add` with `memory_order_relaxed`.
- Reason about when relaxed ordering is safe and when it is not.

## Task Summary

1. **`ReadyFlag`** — a producer writes a non-atomic `int` and then sets an `atomic<bool>` with *release* semantics. A consumer spins on the flag with *acquire* semantics, guaranteeing it sees the written value.
2. **`RelaxedCounter`** — a simple counter using `fetch_add` with `memory_order_relaxed`, demonstrating that atomicity alone (without ordering) suffices for an independent counter.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `ReadyFlag` class | `atomic<bool> ready_` (initially `false`) and a non-atomic `int value_`. |
| `publish(int v)` | Stores `v` into `value_`, then sets `ready_` to `true` with `memory_order_release`. |
| `try_consume()` | Loads `ready_` with `memory_order_acquire`; if `true`, returns `value_` as `std::optional<int>`; otherwise returns `std::nullopt`. |
| `RelaxedCounter` class | `atomic<int> count_` initialised to 0. |
| `increment()` | `fetch_add(1, memory_order_relaxed)`. Marked `noexcept`. |
| `value()` | `count_.load(memory_order_relaxed)`. Marked `const noexcept`. |

## How to Run

From the repository root:

```bash
make day=28 test       # build and run tests
make day=28 asan       # AddressSanitizer build
make day=28 valgrind   # Valgrind memcheck
make day=28 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Implement `ReadyFlag` and `RelaxedCounter` in `exercise.cpp`.
4. Run `make day=28 test` and iterate until all assertions pass.
5. Run the ASan and Valgrind targets to confirm no data races or leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.

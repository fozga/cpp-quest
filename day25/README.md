# Asynchronous Results

## Day Overview

`std::promise` and `std::future` form a one-shot communication channel between a
producer thread and a consumer thread. The producer writes a value (or an exception)
into the promise; the consumer retrieves it through the paired future. This pattern
is the fundamental building block for waiting on asynchronous computations, and is
what higher-level abstractions like `std::async` and thread pools are built upon.

## Learning Objectives

- Create a `std::promise<T>` / `std::future<T>` pair and understand their relationship.
- Pass a `std::promise` into a worker thread so it can deliver a result asynchronously.
- Use `future::get()` to block until the result is available.
- Propagate exceptions across threads with `promise::set_exception()`.
- Recognise that both promise and future are single-use: a second `set_value()` or `get()` is undefined / throws.
- Use `std::shared_ptr<std::promise<T>>` to safely share ownership with a detached thread.
- Copy data into the thread closure to avoid dangling references when the caller's scope ends.
- Distinguish `std::future` from `std::shared_future` for multi-consumer scenarios.

## Task Summary

Implement `async_accumulate` and `async_accumulate_ll`, each of which spawns a
worker thread that sums a `std::vector<int>` and delivers the result through a
`std::future`. The functions return the future immediately so the caller can
continue other work and retrieve the result later with `get()`.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `async_accumulate` | Takes `const std::vector<int>&`, returns `std::future<int>`. |
| `async_accumulate_ll` | Takes `const std::vector<int>&`, returns `std::future<long long>`. |
| Promise lifetime | Use `std::shared_ptr<std::promise<T>>` so the promise outlives the detached thread. |
| Data safety | Copy the vector into the thread's lambda capture — never reference the caller's data. |
| Correctness | Empty vectors must produce a sum of `0`. |

## How to Run

From the repository root:

```bash
make day=25 test       # build and run tests
make day=25 asan       # AddressSanitizer build
make day=25 valgrind   # Valgrind memcheck
make day=25 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Implement both functions in `exercise.cpp`.
4. Run `make day=25 test` and iterate until all assertions pass.
5. Run the ASan and Valgrind targets to confirm no data races or leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.

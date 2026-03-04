# Modern Threads

## Day Overview

`std::jthread` (C++20) auto-joins on destruction, eliminating the most common source of
`std::terminate` calls caused by forgetting to join a `std::thread`. Combined with
`std::stop_token` for cooperative cancellation, `jthread` is the building block for
higher-level patterns like futures, task queues, and periodic workers.

## Learning Objectives

- Understand why `std::jthread` replaces `std::thread` as the default choice in C++20.
- Explain how `std::jthread` auto-joins in its destructor after requesting a stop.
- Use `std::stop_token` and `stop_requested()` for cooperative cancellation.
- Build a worker loop that periodically checks its stop token before continuing.
- Design a `PeriodicWorker` class that owns a `jthread` and tracks iteration count.
- Use `std::atomic` for safe, lock-free counter updates across threads.
- Recognise that the destructor of `jthread` calls `request_stop()` then `join()`.

## Task Summary

Implement a `PeriodicWorker` class that:

1. Accepts an interval (`std::chrono::milliseconds`) and a callable (`std::function<void()>`).
2. Spawns an internal `std::jthread` that repeatedly sleeps for the interval, then invokes the callable.
3. Tracks how many times the callable has been invoked via an `iterations()` accessor.
4. On destruction, requests a stop and lets `jthread` auto-join cleanly.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| Constructor | `PeriodicWorker(Duration interval, std::function<void()> task)` — launches the worker thread immediately. |
| Worker loop | Sleeps for `interval_`, checks `stop_requested()`, calls `task_()`, increments counter. |
| `iterations()` | Returns the current iteration count (`std::size_t`). Thread-safe via `std::atomic`. |
| Destructor | Calls `request_stop()` on the `jthread`; the `jthread` destructor auto-joins. |
| Copy semantics | Deleted — a worker owns a unique thread. |

## How to Run

From the repository root:

```bash
make day=24 test       # build and run tests
make day=24 asan       # AddressSanitizer build
make day=24 valgrind   # Valgrind memcheck
make day=24 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study the header `exercise.hpp` to understand the public interface.
3. Implement the constructor and worker loop in `exercise.cpp`.
4. Run `make day=24 test` and iterate until all assertions pass.
5. Run the ASan and Valgrind targets to confirm no data races or leaks.
6. Answer the self-check questions in `notes.md` without peeking at references.

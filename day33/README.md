# C++23 Deducing This

## Day Overview

Unit tests tell you **what** broke, but dynamic debugging tools tell you **why**.
This day introduces two complementary tools — **AddressSanitizer (ASan)** and
**Valgrind** — that detect memory bugs your tests alone cannot catch: out-of-bounds
access, use-after-free, double deletes, and memory leaks. By running the same
test binary under these tools you get precise diagnostics — stack traces, source
lines, and allocation histories — that point straight to the root cause.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Explain what AddressSanitizer checks at runtime and how it is enabled (`-fsanitize=address`). |
| 2 | Explain what Valgrind's Memcheck tool checks and how it differs from ASan. |
| 3 | Recognise typical ASan error messages (heap-buffer-overflow, use-after-free, leak). |
| 4 | Recognise typical Valgrind error messages (Invalid read/write, definitely lost). |
| 5 | Use `make asan` and `make valgrind` targets to run the test binary under each tool. |
| 6 | Read a stack trace to identify the offending source file and line number. |
| 7 | Understand why the `-g` and `-O0` flags are important for useful diagnostics. |
| 8 | Fix common memory bugs — missing `delete[]`, dangling pointers, off-by-one access. |

## Task Summary

Implement a `Buffer` class that manages a dynamically allocated `int` array with
RAII semantics. The **original** version of the class contained a bug: the
destructor forgot to `delete[]` the internal allocation, causing a memory leak.
The corrected solution in `exercise.cpp` fixes this and documents the bug history
in comments. Running the tests under ASan or Valgrind confirms the fix.

## Implementation Requirements

### `Buffer` class

- **RAII**: the constructor allocates with `new int[capacity]`; the destructor
  releases with `delete[] data_`.
- **`push_back(int)`**: appends a value (asserts that `size_ < capacity_`).
- **`operator[](std::size_t)`**: element access (asserts in-bounds).
- **Move semantics**: move constructor and move-assignment transfer ownership and
  null-out the source. Copy operations are deleted.
- **Bug comment**: `exercise.cpp` contains a comment block describing the
  original leak bug and how the corrected version fixes it.

## How to Run

```bash
# From the repository root
make day=33 test      # compile and run tests
make day=33 asan      # compile with AddressSanitizer and run
make day=33 valgrind  # run under Valgrind leak checker
make day=33 clean     # remove binaries
```

## Suggested Workflow

1. Read `exercise.hpp` to understand the `Buffer` interface.
2. Study `tests.cpp` to see the expected behaviour for construction, push_back,
   element access, move construction, and move assignment.
3. Implement (or review) `exercise.cpp`, paying attention to the destructor and
   move operations.
4. Run `make day=33 test` to verify functional correctness.
5. Run `make day=33 asan` — if you see a `LeakSanitizer` report, the destructor
   is not releasing memory.
6. Run `make day=33 valgrind` — look for "definitely lost" bytes in the summary.
7. Fix the bug, re-run both tools, and confirm a clean report.

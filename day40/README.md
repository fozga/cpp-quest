# Lock-Free Optimization

## Overview

This is the **capstone exercise** of the course — congratulations on reaching the finish line! Today the focus is on **correctness, clarity, and thorough testing**. You will take the TaskList API designed on Day 39 and ensure every path — success and failure — is exercised by a comprehensive test suite.

This project ties together the themes of the entire course: RAII ownership, value semantics, `const`-correct interfaces, and explicit error handling with return values and `std::optional`.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Implement the full behavior of every TaskList operation. |
| 2 | Handle both success and failure paths (missing IDs, empty lists). |
| 3 | Write comprehensive tests covering normal usage, edge cases, and state transitions. |
| 4 | Run the program under sanitizers to verify memory safety. |

## Task — Fully Implement and Test the TaskList API

The header (`exercise.hpp`) and implementation (`exercise.cpp`) are identical to Day 39. Your job today is to write **thorough tests** in `tests.cpp` that cover:

* Adding multiple tasks with different statuses and optional due dates.
* Listing all tasks and filtering by status.
* Finding tasks by ID — both hits and misses.
* Removing tasks — including double-remove and removing non-existent IDs.
* Operating on a completely empty `TaskList`.

Every assertion that passes prints a `[PASS]` line so you can see progress at a glance.

## How to Run

```bash
cd day40/solution
make          # build + run tests
make asan     # AddressSanitizer build
make valgrind # Valgrind memory check
make clean    # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` for tips on writing complete test suites.
2. Review `exercise.hpp` to recall the full API surface.
3. Study `tests.cpp` — run it and confirm every `[PASS]` line prints.
4. Try `make asan` to verify there are no memory errors.
5. Celebrate — you have completed the course!

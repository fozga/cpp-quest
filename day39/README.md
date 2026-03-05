# Final Project – Design

## Day Overview

Before writing a single line of implementation, **API-first design** asks you to describe the public surface of your code — the types callers will use, the functions they will call, and the contracts those functions uphold. Designing before implementing clarifies responsibilities, exposes missing operations early, and provides a natural roadmap for tests.

Today you will draft the API for a small **TaskList** tracker and produce a compilable skeleton that proves every declaration has a matching definition.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Draft a public API for a small domain before implementing logic. |
| 2 | Identify the core entities and their relationships (Task, TaskList). |
| 3 | Plan for testability by choosing return types that are easy to assert on. |
| 4 | Decide where RAII, value semantics, and error handling will appear. |

## Task Summary

Define two types inside `exercise.hpp`:

* **`Task`** — a value type with fields `id` (int), `title` (string), `status` (enum: Todo / InProgress / Done), and an optional `due_date` (string).
* **`TaskList`** — a collection that supports:
  - `add_task(title, status?, due?)` → returns a reference to the newly added task (auto-assigns a unique `id`).
  - `remove_task(id)` → returns `true` if a task was erased.
  - `find_task(id)` → returns a pointer (or `nullptr` if not found), with a `const` overload.
  - `list_all()` → returns a copy of every task.
  - `list_by_status(status)` → returns only the tasks matching the given status.
  - `size()` → returns the current task count.

Provide stub (or full) implementations in `exercise.cpp` so the project compiles cleanly. The test file `tests.cpp` validates that the skeleton links and basic operations work.

## How to Run

From the repository root:

```bash
make day=39 test       # build and run tests
make day=39 asan       # AddressSanitizer build
make day=39 valgrind   # Valgrind memcheck
make day=39 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand API-first design principles.
2. Study `exercise.hpp` — note the enum, the struct, and the class interface.
3. Implement every declared function in `exercise.cpp`.
4. Run `make day=39 test` and iterate until all assertions pass with zero warnings.
5. Review `materials/links.md` for further reading on interface design.

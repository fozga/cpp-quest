# Pure RAII & Rule of 5

## Day Overview

Welcome to **Day 1** of the 40-day C++ quest. Today you tackle the most
foundational idiom in modern C++: **RAII** (Resource Acquisition Is
Initialization) paired with the **Rule of 5**.

Every C++ program that manages a resource — heap memory, file descriptors,
sockets, mutexes, GPU buffers — must answer two questions:

1. **When is the resource acquired?**
2. **When is it released, and who is responsible?**

RAII answers both at once: a constructor acquires the resource, a destructor
releases it, and the object's lifetime *is* the resource's lifetime. Because
C++ destructors run deterministically when an object leaves scope — even when
an exception unwinds the stack — RAII gives you **resource safety** and
**exception safety** as a direct consequence of ordinary scoping rules.

The Rule of 5 tells you *which special member functions* you must implement
(or explicitly delete) once your class manages a raw resource that the
compiler-generated defaults cannot handle correctly.

Together, RAII + Rule of 5 eliminate an entire class of bugs: leaks,
double-frees, and use-after-close.

---

## Learning Objectives

By the end of this day you will be able to:

- Define RAII and explain why it ties resource lifetime to object lifetime.
- List the five special member functions covered by the Rule of 5.
- Explain the Rule of 0 and when it is preferable to the Rule of 5.
- Identify when a class *must* provide custom special members (i.e., when it
  owns a raw resource such as a file descriptor or raw pointer).
- Implement move construction and move assignment that transfer ownership
  without copying.
- Justify deleting the copy constructor and copy assignment operator for
  unique-ownership resources.
- Write a destructor that is `noexcept` and never leaks, even in the presence
  of exceptions.
- Reason about exception safety guarantees (no-throw, strong, basic).

---

## Task Summary

Implement a RAII wrapper class called `FileHandle` that manages a POSIX-like
file descriptor (an `int` handle). The class must:

- Follow the **Rule of 5** exactly.
- Use **-1** as the invalid/sentinel value.
- Be **exception-safe** and **never leak** a descriptor.
- Provide `is_valid`, `get`, and `reset` operations.
- Use a pair of free helper functions (`open_pseudo_file` /
  `close_pseudo_file`) that simulate OS open/close system calls.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Invalid sentinel | `-1` (matches POSIX convention) |
| Default constructor | Creates an invalid handle (`fd_ == -1`) |
| Explicit constructor | Takes an `int` handle; stores it directly |
| Destructor | Calls `close_pseudo_file` if valid; never throws |
| Copy operations | **Deleted** (unique ownership) |
| Move operations | `noexcept`; transfer ownership, set source to `-1` |
| `is_valid()` | Returns `true` when `fd_ >= 0` |
| `get()` | Returns the descriptor; throws `std::runtime_error` if invalid |
| `reset(int)` | Closes the old handle (if valid), then stores the new one |
| `open_pseudo_file` | Returns a monotonically increasing non-negative `int` |
| `close_pseudo_file` | No-op for `-1`; otherwise logically closes the descriptor |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=01 test

# Build with AddressSanitizer and run
make day=01 asan

# Build a normal binary then run under Valgrind
make day=01 valgrind

# Remove compiled artefacts
make day=01 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand RAII and the Rule of 5.
2. **Open** `solution/exercise.hpp` and study the class interface.
3. **Implement** each member function in `solution/exercise.cpp`, starting
   with the constructor and destructor, then the move operations, then
   `reset()` and `get()`.
4. **Compile** with `make day=01 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=01 asan`) and Valgrind
   (`make day=01 valgrind`) to confirm zero leaks.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.

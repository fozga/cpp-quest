# Vtables & Memory Leaks

## Day Overview

Welcome to **Day 8** of the 40-day C++ quest. Today you explore **dynamic
polymorphism**, the mechanism that powers it (**vtables** and **vptrs**), and
the surprisingly common bug that results from deleting a derived object through
a base pointer when the base class destructor is **not virtual**.

When you call a virtual function through a base pointer, the compiler uses the
object's hidden **vptr** to index into a **vtable** — a per-class table of
function pointers — and dispatches to the correct override at run time. This is
the backbone of runtime polymorphism in C++.

Destructors participate in the same dispatch mechanism. If a base class
destructor is not virtual and you `delete` a derived object through a base
pointer, only the base destructor runs. The derived destructor — and any
resources it manages — is silently skipped. The result is a **memory leak** (or
worse: leaked file handles, unclosed sockets, dangling state).

The fix ties directly back to **RAII**: give polymorphic base classes a
`virtual` destructor so that destruction always begins at the most-derived
class and proceeds upward, releasing every resource along the way.

---

## Learning Objectives

By the end of this day you will be able to:

- Describe how a vtable is structured and how vptrs enable dynamic dispatch.
- Explain why a polymorphic base class must declare a virtual destructor.
- Trace the destruction order of an inheritance hierarchy (most-derived first).
- Identify symptoms of a missing virtual destructor: leaked heap memory,
  resource handles left open, sanitizer or Valgrind warnings.
- Fix a leaking hierarchy by adding `virtual ~Base()` and `override` on
  derived destructors.
- Articulate the cost of virtual dispatch (vptr per object, indirect call) and
  when that cost is acceptable.
- Connect the fix to the broader RAII principle: deterministic cleanup through
  destructors.

---

## Task Summary

You are given a small logging hierarchy: a `BaseLogger` with two derived
classes, `FileLogger` and `ConsoleLogger`. `FileLogger` owns a heap-allocated
`std::string*` representing a filename. In the **buggy** version the base
destructor is non-virtual, so deleting a `FileLogger` through a `BaseLogger*`
skips `FileLogger::~FileLogger` and leaks the string.

Your task: fix the hierarchy by making the base destructor virtual, confirm
that a live-object counter drops to zero after cleanup, and verify the fix
under AddressSanitizer and Valgrind.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Base class | `BaseLogger` with `virtual void log(...)` and `virtual ~BaseLogger()` |
| Derived 1 | `FileLogger` — owns a `std::string*` on the heap (`filename_`) |
| Derived 2 | `ConsoleLogger` — lightweight, no heap resources |
| Factory functions | `make_file_logger`, `make_console_logger` return `BaseLogger*` |
| Cleanup function | `use_and_destroy_all` — logs with each logger, deletes every pointer, clears the vector |
| Tracking | `get_live_file_logger_count()` — returns how many `FileLogger` objects are alive |
| Solution | The corrected version with `virtual ~BaseLogger()` and comments describing the original bug |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=08 test

# Build with AddressSanitizer and run
make day=08 asan

# Build a normal binary then run under Valgrind
make day=08 valgrind

# Remove compiled artefacts
make day=08 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand vtables, vptrs, and virtual
   destructors.
2. **Open** `solution/exercise.hpp` and study the class hierarchy.
3. **Implement** each member function in `solution/exercise.cpp`, starting
   with `BaseLogger`, then `FileLogger` (pay attention to the heap resource),
   then `ConsoleLogger`, then the free functions.
4. **Compile** with `make day=08 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass. The critical assertion is
   that `get_live_file_logger_count() == 0` after `use_and_destroy_all`.
6. **Run** under ASan (`make day=08 asan`) and Valgrind
   (`make day=08 valgrind`) to confirm zero leaks.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.

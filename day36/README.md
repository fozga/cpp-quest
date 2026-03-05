# Library Boundaries

## Day Overview

Real-world C++ libraries expose **public interfaces** while hiding their implementation details. Clean library boundaries mean consumers depend only on declarations in headers, never on internal data structures or helper functions buried in source files. Header/source separation is the primary mechanism: headers declare *what* a library can do; source files define *how* it does it.

When boundaries are drawn well, implementation changes never force recompilation of client code, internal refactors stay invisible, and the public surface area remains small and stable.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Distinguish public API (header declarations) from internal implementation (source definitions). |
| 2 | Design a minimal header that exposes only what clients need. |
| 3 | Hide implementation details in `.cpp` using the pImpl (pointer-to-implementation) idiom. |
| 4 | Test a library exclusively through its public API. |

## Task Summary

Build a **StringInterner** class that maps strings to compact integer IDs and back:

* `intern(s)` — insert a string (or return its existing ID).
* `has_string(s)` / `has_id(id)` — membership queries.
* `get_string(id)` / `get_id(s)` — bidirectional lookup (throw on missing).
* `size()` — number of unique strings interned.

The header (`exercise.hpp`) must expose **only** the public interface and a forward-declared `Impl` struct behind a `std::unique_ptr`. All data structures (`unordered_map`, `vector`) live exclusively in `exercise.cpp`.

The class must be **move-only** (copy operations deleted, move operations defaulted in the source file).

## How to Run

From the repository root:

```bash
make day=36 test       # build and run tests
make day=36 asan       # AddressSanitizer build
make day=36 valgrind   # Valgrind memcheck
make day=36 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand library boundary concepts and the pImpl idiom.
2. Study the header in `exercise.hpp` — note what is and isn't included.
3. Implement the `Impl` struct and all member functions in `exercise.cpp`.
4. Run `make day=36 test` and iterate until all assertions pass with zero warnings.
5. Review `materials/links.md` for deeper reading.

# Enforcing RVO

## Day Overview

Welcome to **Day 6** of the 40-day C++ quest. Today you explore **copy
elision**, **RVO** (Return Value Optimization), and **NRVO** (Named Return
Value Optimization) — the compiler techniques that make returning objects by
value essentially free.

In C++17 and later, returning a prvalue (a temporary) by value is
**guaranteed** to be elided: no copy, no move — the object is constructed
directly in the caller's storage. Even named local variables benefit from
NRVO in practice, although the standard does not mandate it.

By designing a class that **deletes its copy operations** while keeping
move operations intact, you can *enforce* that the compiler uses elision or
move semantics. If it tried to copy, the program would not compile. This is
a powerful technique for expressing unique-ownership value types, builder
objects, and fluent APIs that remain efficient without any manual
`std::move` gymnastics on the caller side.

---

## Learning Objectives

By the end of this day you will be able to:

- Distinguish between **RVO** (unnamed return value optimization) and
  **NRVO** (named return value optimization).
- Explain when copy elision is **guaranteed** (C++17 mandatory elision for
  prvalues) versus merely **permitted** (NRVO).
- Describe how returning by value interacts with move semantics: the
  compiler tries elision first, then implicit move, then copy.
- Design **non-copyable but returnable** types that rely on elision and
  move semantics.
- Implement a **fluent API** using method chaining with `return *this` by
  reference.
- Build a factory function that returns a non-copyable object by value,
  relying entirely on RVO.
- Explain why `noexcept` move operations are important for containers and
  for enabling optimizations.

---

## Task Summary

Implement a `QueryBuilder` class that models a simple SQL query builder
with a **fluent chaining** interface. The class is **non-copyable** (copy
constructor and copy assignment are deleted) and **move-only** (move
operations are `noexcept`). A static factory function `from_table` returns
a `QueryBuilder` by value, relying on RVO. A free function
`make_sample_query` demonstrates returning a named local variable (NRVO).

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Private data | `std::vector<std::string> selects_`, `std::string table_`, `std::vector<std::string> conditions_`, `int limit_ = -1` |
| Default ctor | `= default` |
| Destructor | `= default` |
| Copy operations | **Deleted** (enforces move-only / elision) |
| Move operations | `noexcept`; transfer all members, reset source `limit_` to `-1` |
| `from_table(string)` | Static factory — creates a local `QueryBuilder`, sets `table_`, returns by value |
| `select(string)` | Pushes column to `selects_`, returns `*this` by reference |
| `where(string)` | Pushes condition to `conditions_`, returns `*this` by reference |
| `limit(int)` | Sets `limit_`, returns `*this` by reference |
| `str() const` | Builds pseudo-SQL: `SELECT col1, col2 FROM table WHERE c1 AND c2 LIMIT N` |
| `make_sample_query()` | Free function — builds a query on `"users"` with selects, where, limit; returns by value |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=06 test

# Build with AddressSanitizer and run
make day=06 asan

# Build a normal binary then run under Valgrind
make day=06 valgrind

# Remove compiled artefacts
make day=06 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand copy elision, RVO, and NRVO.
2. **Open** `solution/exercise.hpp` and study the class interface — note
   the deleted copy operations.
3. **Implement** each member function in `solution/exercise.cpp`, starting
   with the move operations, then the factory and fluent methods, then
   `str()`.
4. **Compile** with `make day=06 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=06 asan`) and Valgrind
   (`make day=06 valgrind`) to confirm zero leaks.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.

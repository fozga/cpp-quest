# Variadic Templates

## Day Overview

Welcome to **Day 20** of the 40-day C++ quest. Today you explore **variadic
templates** — one of the most powerful features in modern C++. Variadic
templates let you write functions and classes that accept an arbitrary number of
arguments of arbitrary types. Combined with **fold expressions** (C++17), they
replace many uses of C-style variadic functions and recursive template
expansions with concise, type-safe code.

---

## Learning Objectives

By the end of this day you will be able to:

- Understand the `typename... Ts` syntax for declaring template parameter packs.
- Understand function parameter packs (`const Ts&... args`) and how they
  capture arguments.
- Use `sizeof...(Ts)` to query the number of elements in a pack at compile
  time.
- Write fold expressions to expand parameter packs inline (e.g.,
  `((os << args << ' '), ...)`).
- Distinguish between unary folds and binary folds, and know which operators
  they support.
- Implement a variadic `log_line` function that serializes any number of
  arguments to an output stream.
- Implement a convenience `log_to_cout` wrapper that forwards its arguments to
  `log_line`.
- Recognise common pitfalls: empty packs, operator precedence in folds, and
  overloading conflicts.

---

## Task Summary

Implement three function templates in `exercise.hpp`:

1. **`log_line(std::ostream& os, const Ts&... args)`** — uses a fold expression
   to write each argument to `os` followed by a space, then appends a newline
   (`'\n'`).
2. **`log_to_cout(const Ts&... args)`** — convenience wrapper that calls
   `log_line(std::cout, args...)`.
3. **`arg_count(const Ts&...)`** — returns `sizeof...(Ts)` as `std::size_t`.

---

## Implementation Requirements

- All three functions are templates and **must** be defined in the header
  (`exercise.hpp`).
- `log_line` must use the fold expression `((os << args << ' '), ...)` to
  expand the pack.
- `log_line` must write a trailing newline character after all arguments.
- `exercise.cpp` should include the header but otherwise remain minimal (no
  function definitions).
- The code must compile cleanly with `g++ -std=c++20 -Wall -Wextra -O0 -g`
  and produce zero warnings.

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=20 test

# Build with AddressSanitizer and run
make day=20 asan

# Build a normal binary then run under Valgrind
make day=20 valgrind

# Remove compiled artefacts
make day=20 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand parameter packs and fold
   expressions.
2. **Open** `solution/exercise.hpp` and implement `log_line` using a fold
   expression.
3. **Add** `log_to_cout` and `arg_count`.
4. **Compile** with `make day=20 test` — fix any compiler errors.
5. **Run** the tests and verify all assertions pass.
6. **Run** under ASan (`make day=20 asan`) and Valgrind
   (`make day=20 valgrind`) to confirm clean execution.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper exploration.

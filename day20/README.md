# Variadic Templates

## Day Overview

Day 20 explores **variadic templates** — one of the most powerful features in modern C++. Variadic templates let you write functions and classes that accept an arbitrary number of arguments of arbitrary types. Combined with **fold expressions** (C++17), they replace many uses of C-style variadic functions and recursive template expansions with concise, type-safe code.

## Learning Objectives

- Understand the `typename... Ts` syntax for declaring template parameter packs.
- Understand function parameter packs (`const Ts&... args`) and how they capture arguments.
- Use `sizeof...(Ts)` to query the number of elements in a pack at compile time.
- Write fold expressions to expand parameter packs inline (e.g., `((os << args << ' '), ...)`).
- Distinguish between unary folds and binary folds, and know which operators they support.
- Implement a variadic `log_line` function that serializes any number of arguments to an output stream.
- Implement a convenience `log_to_cout` wrapper that forwards its arguments to `log_line`.
- Recognise common pitfalls: empty packs, operator precedence in folds, and overloading conflicts.

## Task Summary

Implement three function templates in `exercise.hpp`:

1. **`log_line(std::ostream& os, const Ts&... args)`** — uses a fold expression to write each argument to `os` followed by a space, then appends a newline (`'\n'`).
2. **`log_to_cout(const Ts&... args)`** — convenience wrapper that calls `log_line(std::cout, args...)`.
3. **`arg_count(const Ts&...)`** — returns `sizeof...(Ts)` as `std::size_t`.

## Implementation Requirements

- All three functions are templates and **must** be defined in the header (`exercise.hpp`).
- `log_line` must use the fold expression `((os << args << ' '), ...)` to expand the pack.
- `log_line` must write a trailing newline character after all arguments.
- `exercise.cpp` should include the header but otherwise remain minimal (no function definitions).
- The code must compile cleanly with `g++ -std=c++20 -Wall -Wextra -O0 -g` and produce zero warnings.

## How to Run

From the repository root:

```bash
make day=20 test       # build and run tests
make day=20 asan       # build with AddressSanitizer and run
make day=20 valgrind   # build normally, then run under Valgrind
make day=20 clean      # remove built binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` to understand parameter packs and fold expressions.
2. Open `exercise.hpp` and implement `log_line` using a fold expression.
3. Add `log_to_cout` and `arg_count`.
4. Run `make day=20 test` and verify all assertions pass.
5. Run `make day=20 asan` and `make day=20 valgrind` to confirm no memory issues.
6. Review the self-check questions in `materials/notes.md`.

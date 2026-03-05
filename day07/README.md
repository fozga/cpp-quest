# Perfect Forwarding

## Day Overview

When writing generic wrapper or factory functions in C++, arguments must be
passed through to an inner function or constructor **without losing their value
category**. A temporary (`rvalue`) passed to a wrapper should still arrive as an
rvalue at its final destination; an lvalue should remain an lvalue. Getting this
wrong silently forces extra copies or, worse, moves from objects the caller
still needs.

**Perfect forwarding** solves this problem. It relies on two language features
introduced in C++11:

1. **Forwarding (universal) references** — `T&&` in a deduced template context,
   which can bind to both lvalues and rvalues.
2. **`std::forward<T>`** — a conditional cast that restores the original value
   category based on how `T` was deduced.

Together they let you write functions like `std::make_unique`,
`std::make_shared`, and `std::emplace_back` that construct objects in-place with
zero unnecessary copies or moves. This connects directly to the move semantics
explored in earlier days: perfect forwarding is how the standard library
*delivers* arguments to the move (or copy) constructors you wrote.

## Learning Objectives

- Define what a forwarding (universal) reference is and how it differs from a
  plain rvalue reference.
- Explain reference collapsing rules and why `T&&` with a deduced `T` can bind
  to both lvalues and rvalues.
- Describe the role of `std::forward<T>` and why simply using `std::move` would
  be incorrect in a forwarding context.
- Identify the common pitfalls: double-move, forwarding inside loops,
  incorrect lambda captures of forwarding references, and dangling-reference
  hazards.
- Implement a generic factory function (`make_with_log`) that constructs any
  type `T` with perfectly forwarded arguments.
- Implement a generic call wrapper (`call_with_log`) that invokes any callable
  with perfectly forwarded arguments and preserves the return type via
  `decltype(auto)`.
- Use `static_assert` and `std::is_same_v` to verify value categories at
  compile time.
- Test that lvalue arguments are **not** moved from after being forwarded.

## Task Summary

Implement two function templates in `exercise.hpp`:

| Function | Signature | Behaviour |
|---|---|---|
| `make_with_log<T>` | `template <typename T, typename... Args> T make_with_log(Args&&... args)` | Increments a global log counter, prints a log message, and constructs a `T` by perfectly forwarding `args`. |
| `call_with_log` | `template <typename F, typename... Args> decltype(auto) call_with_log(F&& f, Args&&... args)` | Increments a global log counter, prints a log message, and invokes `f` with perfectly forwarded `args`, returning whatever `f` returns. |

A small `Widget` struct is provided for testing construction through
`make_with_log`.

## Implementation Requirements

- `make_with_log` accepts `Args&&...` and uses `std::forward<Args>(args)...` to
  construct `T`.
- `call_with_log` uses `decltype(auto)` as its return type so the exact return
  type (including references) of the callable is preserved.
- Both functions call `increment_log_count()` (defined in `exercise.cpp`) so
  that tests can verify the number of forwarding operations that occurred.
- `Widget` has a default constructor and a parameterised constructor
  (`std::string name, int value`). Constructors are defined in `exercise.cpp`.
- Template functions are **defined in the header** (`exercise.hpp`); non-template
  functions are defined in `exercise.cpp`.

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=07 test

# Build with AddressSanitizer and run
make day=07 asan

# Build a normal binary then run under Valgrind
make day=07 valgrind

# Remove compiled artefacts
make day=07 clean
```

## Suggested Workflow

1. Read `materials/notes.md` for background on forwarding references and
   `std::forward`.
2. Open `solution/exercise.hpp` and study the `Widget` struct and the two
   function-template signatures.
3. Implement `make_with_log` first — it is the simpler of the two.
4. Run the tests (`make day=07 test`) and confirm construction forwarding works
   for both lvalue and rvalue arguments.
5. Implement `call_with_log`, paying attention to the `decltype(auto)` return.
6. Verify that lvalue arguments survive forwarding unmodified (the test checks
   this).
7. Run under ASan and Valgrind to confirm no leaks or undefined behaviour.

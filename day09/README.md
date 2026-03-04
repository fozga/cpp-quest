# Safe Subclassing

## Day Overview

Welcome to **Day 9** of the 40-day C++ quest. Today you tackle two small
but powerful specifiers that make inheritance hierarchies **safer** and
**self-documenting**: `override` and `final`.

Inheritance is one of the cornerstones of object-oriented C++, but it comes
with subtle pitfalls:

1. **Accidental hiding** — you intend to override a base-class virtual
   function but mistype the parameter list, add an extra `const`, or change
   the return type. The compiler silently creates a *new* function instead
   of overriding the old one, and polymorphic calls never reach your code.
2. **Unintended subclassing** — a class was designed to be a concrete leaf
   in the hierarchy, yet nothing in the language prevents someone from
   deriving from it and overriding its behaviour in ways the original author
   never anticipated.

C++11 introduced `override` and `final` to address both problems:

- `override` tells the compiler "I intend to override a virtual function in
  a base class — reject this code if no such function exists."
- `final` tells the compiler "no derived class may override this function"
  (when applied to a virtual function) or "no class may derive from this
  class" (when applied to the class itself).

Together they turn latent run-time bugs into compile-time errors.

---

## Learning Objectives

By the end of this day you will be able to:

- Apply the `override` specifier to every overriding virtual function.
- Explain how `override` turns silent signature mismatches into compiler
  errors.
- Use `final` on a virtual function to prevent further overriding in
  deeper subclasses.
- Use `final` on a class to prevent any further derivation.
- Articulate design reasons for marking a class or method `final` (safety,
  optimisation hints, intent documentation).
- Build a three-level inheritance hierarchy (Widget → Button →
  ToggleButton) that compiles cleanly with `-Wall -Wextra`.
- Refactor an existing hierarchy to add `override` and `final` without
  changing observable behaviour.

---

## Task Summary

Implement a small **UI widget hierarchy** with three levels:

| Class | Inherits From | Notes |
|---|---|---|
| `Widget` | — | Abstract-ish base with `name()` and `draw()` |
| `Button` | `Widget` | Overrides both virtual functions |
| `ToggleButton` | `Button` | `final` class; `draw()` is also `final` |

Two free functions exercise the hierarchy polymorphically:

- `draw_all` — iterate over a vector of `Widget*` and call `draw()`.
- `collect_names` — build a comma-separated string of `name()` results.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| Levels | 3: `Widget` → `Button` → `ToggleButton` |
| `override` usage | Every overriding function must use `override` |
| `final` method | `ToggleButton::draw()` is marked `final` |
| `final` class | `ToggleButton` itself is marked `final` |
| Virtual destructor | `Widget` provides `virtual ~Widget() = default` |
| Comments | Each use of `override` / `final` has a brief comment explaining intent |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=09 test

# Build with AddressSanitizer and run
make day=09 asan

# Build a normal binary then run under Valgrind
make day=09 valgrind

# Remove compiled artefacts
make day=09 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand `override`, `final`, and
   the problems they solve.
2. **Open** `solution/exercise.hpp` and study the three-class hierarchy.
3. **Implement** each member function in `solution/exercise.cpp`, starting
   with `Widget`, then `Button`, then `ToggleButton`, and finally the two
   free functions.
4. **Compile** with `make day=09 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=09 asan`) and Valgrind
   (`make day=09 valgrind`) to confirm zero leaks.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.

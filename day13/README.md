# Type-Safe Unions

## Day Overview

Welcome to **Day 13** of the 40-day C++ quest. Today you explore
**`std::variant`** — the modern, type-safe replacement for C-style unions — and
**`std::visit`**, the standard mechanism for processing the active alternative.

A `std::variant<Ts...>` holds exactly one value from a closed set of types at
any given time. Unlike a raw `union`, it tracks which type is active, prevents
access through the wrong type, and never exhibits undefined behaviour when used
correctly. These properties make `std::variant` the natural tool for **data
modelling** (sum types, AST nodes, protocol messages) and a building block for
**error handling** (see Day 14's `std::expected`).

---

## Learning Objectives

By the end of this day you will be able to:

- Explain what `std::variant` represents and how it differs from a C `union`.
- Use `std::get<T>`, `std::get<I>`, and `std::get_if` to access the active
  alternative safely.
- Use `std::holds_alternative<T>` to query the active type at runtime.
- Handle `std::bad_variant_access` when an incorrect alternative is requested.
- Apply `std::visit` with a visitor callable to process all alternatives
  exhaustively.
- Use the *overload pattern* (an aggregate of lambdas) to build ad-hoc
  visitors.
- Model an arithmetic expression AST whose node types are represented as
  variant alternatives.
- Combine `std::variant` with `std::unique_ptr` and recursion to build and
  evaluate tree structures.

---

## Task Summary

Implement a small **arithmetic expression AST** using `std::variant`. The AST
supports three node kinds:

| Node kind        | Meaning                               |
|------------------|---------------------------------------|
| `IntegerLiteral` | A leaf holding a single `int` value   |
| `Add`            | Binary addition of two child nodes    |
| `Mul`            | Binary multiplication of two child nodes |

Each node is a `struct Node` whose `data` member is a
`std::variant<IntegerLiteral, Add, Mul>`. The `Add` and `Mul` structs own
their children through `std::unique_ptr<Node>`.

You must provide:

- **Factory functions** — `make_int`, `make_add`, `make_mul` — that construct
  nodes conveniently.
- **`evaluate`** — recursively computes the integer result of an expression
  tree using `std::visit`.
- **`to_string`** — recursively produces a human-readable string such as
  `"(2 * (3 + 4))"` using `std::visit`.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| `IntegerLiteral` | Stores a single `int value` |
| `Add` / `Mul` | Each stores `std::unique_ptr<Node> left` and `right` |
| `Node` | Contains `std::variant<IntegerLiteral, Add, Mul> data` |
| `make_int(int)` | Returns `Node{IntegerLiteral{v}}` |
| `make_add(Node, Node)` | Moves both children into heap-allocated nodes |
| `make_mul(Node, Node)` | Same as `make_add` but wraps in `Mul` |
| `evaluate(const Node&)` | Uses `std::visit` with a visitor; recurses into children |
| `to_string(const Node&)` | Uses `std::visit`; literals produce `"42"`, binary ops produce `"(l op r)"` |
| Copy semantics | `Node` is move-only (contains `unique_ptr` members) |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=13 test

# Build with AddressSanitizer and run
make day=13 asan

# Build a normal binary then run under Valgrind
make day=13 valgrind

# Remove compiled artefacts
make day=13 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand `std::variant` fundamentals and
   the visitor pattern.
2. **Open** `solution/exercise.hpp` and study the node type definitions.
3. **Implement** the factory functions in `solution/exercise.cpp` first —
   they are the simplest starting point.
4. **Implement** `evaluate` using `std::visit` and a visitor struct or the
   overload pattern.
5. **Implement** `to_string` with a similar visitor approach.
6. **Compile** with `make day=13 test` — fix any compiler errors.
7. **Run** the tests and make every assertion pass.
8. **Run** under ASan (`make day=13 asan`) and Valgrind
   (`make day=13 valgrind`) to confirm zero leaks.
9. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
10. **Browse** the links in `materials/links.md` for deeper understanding.

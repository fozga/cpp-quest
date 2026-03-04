# Optional Values

## Day Overview

Welcome to **Day 12** of the 40-day C++ quest. Today you explore
**`std::optional`** — a vocabulary type that encodes "a value of type `T`, or
nothing" directly in the type system.

In C and pre-C++17 code, the absence of a value is typically communicated
through raw null pointers, out-parameters paired with a boolean flag, or
magic sentinel values like `-1` or `INT_MAX`. Every one of these conventions
is **implicit**: the compiler cannot enforce that the caller checks for the
missing case. A forgotten null check or an unchecked sentinel silently
becomes undefined behaviour or a logic bug.

`std::optional<T>` makes the "maybe absent" semantics **explicit and
type-safe**. It stores the `T` inline (no heap allocation) plus a boolean
flag, which makes it a natural fit for embedded-style or performance-sensitive
code where dynamic allocation is undesirable. It also connects directly to
the safer resource-handling philosophy you practised with RAII: instead of
relying on conventions, let the type system carry the intent.

---

## Learning Objectives

By the end of this day you will be able to:

- Explain what `std::optional<T>` represents and why it exists in C++17.
- Use `has_value()`, `operator bool`, `value()`, `value_or()`, and
  `operator*` to inspect and access an optional's contents.
- Design functions that return `std::optional` instead of using sentinels or
  output parameters.
- Construct an `std::optional` in the engaged and disengaged states, including
  the use of `std::nullopt`.
- Handle failed conversions (string-to-int, string-to-bool) gracefully by
  returning an empty optional rather than throwing or returning a sentinel.
- Discuss the storage model of `std::optional` (inline `T` + flag, no heap)
  and its implications for embedded and real-time contexts.
- Recognise situations where `std::optional` is preferable to
  `std::unique_ptr<T>` and vice versa.
- Evaluate trade-offs when `T` is large and the optional copy/move cost
  matters.

---

## Task Summary

Implement an embedded-friendly **configuration parser** that reads
`key=value` text lines and stores them in a simple `Config` structure. Provide
lookup functions that return:

- `std::optional<std::string>` for raw string values,
- `std::optional<int>` for integer values (parsed safely), and
- `std::optional<bool>` for boolean values (`true`/`false`/`1`/`0`,
  case-insensitive).

When a key is missing or the value cannot be parsed to the requested type, the
function returns an empty optional — no exceptions, no sentinels.

---

## Implementation Requirements

| Aspect | Detail |
|---|---|
| `ConfigEntry` | `struct` with `std::string key` and `std::string value` |
| `Config` | `struct` with `std::vector<ConfigEntry> entries` |
| `make_config_from_lines` | Accepts `const std::vector<std::string>&`, splits each line at the first `=`, trims whitespace, stores as `ConfigEntry` |
| `get_string` | Linear search for `key`; returns `std::optional<std::string>` (engaged if found, empty otherwise) |
| `get_int` | Calls `get_string`, then parses with `std::from_chars` or `std::stoi`; returns empty optional on missing key or parse failure |
| `get_bool` | Calls `get_string`, accepts `"true"`, `"false"`, `"1"`, `"0"` (case-insensitive); returns empty optional otherwise |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=12 test

# Build with AddressSanitizer and run
make day=12 asan

# Build a normal binary then run under Valgrind
make day=12 valgrind

# Remove compiled artefacts
make day=12 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand `std::optional` and its API.
2. **Open** `solution/exercise.hpp` and study the types and function
   signatures.
3. **Implement** each function in `solution/exercise.cpp`, starting with
   `make_config_from_lines`, then the three lookup functions.
4. **Compile** with `make day=12 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=12 asan`) and Valgrind
   (`make day=12 valgrind`) to confirm zero leaks and no undefined behaviour.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper understanding.

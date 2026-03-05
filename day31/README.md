# Refactoring Safely

## Day Overview

Legacy code is often defined as code without tests — code whose behavior we
cannot verify automatically after making changes. Safe refactoring is the
discipline of restructuring code without changing its observable behavior,
relying on a test suite as a safety net.

Today we practice the most common safe refactoring pattern: taking a monolithic
function that mixes computation with I/O and splitting it into a **pure
computation** function and a thin **I/O wrapper**. Pure functions are easier to
test, easier to compose, and easier to reason about because they depend only on
their inputs and produce only their return value. By extracting them we clarify
data flow, eliminate hidden dependencies, and make the code friendlier to future
change.

## Learning Objectives

- Identify common code smells in monolithic functions: mixed responsibilities,
  hidden data flow, hard-to-test I/O interleaved with logic.
- Apply the **Extract Function** refactoring to separate computation from
  output.
- Introduce a **Parameter Object** (`Stats` struct) to bundle related return
  values into a single, self-documenting type.
- Separate computation from I/O so that the computational core is a pure
  function with no side effects.
- Use a comprehensive test suite as a safety net that proves refactored code
  preserves the original behavior.
- Write deterministic, repeatable tests for pure functions without needing mock
  objects or file redirection.
- Preserve a backward-compatible façade (`legacy_print_stats`) so existing
  callers are unaffected by the internal restructuring.

## Task Summary

Refactor a monolithic `legacy_print_stats` function — which computes summary
statistics and prints them in one pass — into two focused functions:

1. **`compute_stats`** — a pure function that accepts a `const std::vector<int>&`
   and returns a `Stats` struct containing min, max, average, and value-sign
   counts.
2. **`print_stats`** — an I/O function that formats a `Stats` struct to a
   `std::ostream`.

The original `legacy_print_stats` becomes a thin façade that calls
`compute_stats` then `print_stats`, keeping any existing callers working.

## Implementation Requirements

| Requirement | Detail |
|---|---|
| `Stats` struct | Fields: `int min`, `int max`, `double average`, `std::size_t count_positive`, `std::size_t count_negative`, `std::size_t count_zero`, `std::size_t count_total`. All default-initialized. |
| `compute_stats` | `Stats compute_stats(const std::vector<int>& values)` — pure function, no I/O. Returns default `Stats` with `count_total == 0` for empty input. |
| `print_stats` | `void print_stats(const Stats& s, std::ostream& os)` — writes exactly: `count: N\nmin: X\nmax: Y\navg: Z\npos: A\nneg: B\nzero: C\n` |
| `legacy_print_stats` | `void legacy_print_stats(const std::vector<int>& values, std::ostream& os)` — calls `compute_stats` then `print_stats`. |

## How to Run

From the repository root:

```bash
make day=31 test       # build and run tests
make day=31 asan       # AddressSanitizer build
make day=31 valgrind   # Valgrind memcheck
make day=31 clean      # remove binaries
```

## Suggested Workflow

1. Read through `materials/notes.md` and the linked references.
2. Study `exercise.hpp` to understand the `Stats` struct and the three function
   signatures.
3. Implement `compute_stats` as a pure function — handle the empty-vector edge
   case first, then the general case.
4. Implement `print_stats` to format the `Stats` fields exactly as specified.
5. Implement `legacy_print_stats` as a one-liner façade.
6. Run `make day=31 test` and verify all assertions pass.
7. Run the ASan and Valgrind targets to confirm no memory errors.
8. Answer the self-check questions in `notes.md` without peeking at references.

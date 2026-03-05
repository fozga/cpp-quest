# Advanced Lambdas

## Day Overview

Lambdas in C++ are more than inline function literals — they are full closure
objects with captured state stored as data members of a compiler-generated class.
This day explores the mechanics behind capture-by-value and capture-by-reference,
the default `const`-ness of the call operator, how `mutable` unlocks stateful
lambdas, and how capture initializers let you move resources and create new
members on the fly. These techniques are the building blocks for iterators,
ranges, and functional-style pipelines.

## Learning Objectives

- Understand that a lambda expression produces an unnamed closure class with an
  `operator()` and captured variables as data members.
- Distinguish capture-by-value (`[x]`) from capture-by-reference (`[&x]`) and
  know the lifetime implications of each.
- Explain why a lambda's `operator()` is `const` by default and what happens to
  captured-by-value members.
- Use the `mutable` keyword to allow a lambda to modify its captured-by-value
  state, understanding that it modifies the internal copy, not the original.
- Write capture initializers (`[y = std::move(x)]`, `[factor = 2 * base]`) to
  create new data members or move resources into a closure.
- Build a stateful aggregator factory that returns a callable accumulating data
  samples (count, sum, min, max, average).
- Recognise that copying a mutable lambda produces an independent copy whose
  state diverges from the original.

## Task Summary

Implement a **data-aggregator factory**. The factory function `make_aggregator`
returns an `AggregatorHandle` — a struct bundling a callable (`feed`) that
accumulates `double` samples with a `shared_ptr` to internal state. A free
function `get_snapshot` reads the current statistics (count, sum, min, max,
average) from the handle.

The implementation exercises mutable captures, capture initializers, and
shared-pointer-based state management.

## Implementation Requirements

| Component | Purpose |
|---|---|
| `AggregatorState` | POD struct holding `count`, `sum`, `min`, `max` |
| `AggregatorSnapshot` | Read-only snapshot adding `average` |
| `AggregatorImpl` | Internal state container (`AggregatorState` + `label`) |
| `AggregatorHandle` | Public handle with `std::function<void(double)> feed` and `std::shared_ptr<AggregatorImpl> impl` |
| `make_aggregator(label)` | Factory: creates shared impl, returns handle with capturing lambda |
| `get_snapshot(handle)` | Reads current state, computes average, returns snapshot |

## How to Run

From the repository root:

```bash
make day=15 test       # build and run tests
make day=15 asan       # AddressSanitizer build
make day=15 valgrind   # Valgrind memcheck
make day=15 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to review capture modes and mutable lambdas.
2. Study the structs in `exercise.hpp`.
3. Implement `make_aggregator` and `get_snapshot` in `exercise.cpp`.
4. Run `make day=15 test` and iterate until all assertions pass.
5. Run the sanitizer and valgrind targets to verify memory safety.

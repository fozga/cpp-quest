# Dependency Inversion

## Overview

The **Dependency Inversion Principle** (DIP) states that high-level modules should not depend on low-level modules — both should depend on abstractions. In C++ this means coding against abstract base classes (interfaces) rather than concrete types, and injecting those dependencies through constructors. The pattern is often called **ports and adapters**: a *port* is the abstract interface your core logic uses, and an *adapter* is a concrete class that satisfies it (console output, file writer, network client, etc.).

DIP makes code easier to test (swap in a fake), easier to extend (add a new adapter without touching business logic), and more modular overall.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Define the Dependency Inversion Principle and explain why high-level code should depend on abstractions. |
| 2 | Extract an abstract `Logger` interface with a pure-virtual method. |
| 3 | Inject dependencies via constructor reference so the caller controls the lifetime. |
| 4 | Write a test fake (`VectorLogger`) to verify behaviour without side effects. |

## Task — `OrderProcessor` with Abstract `Logger`

Build an **OrderProcessor** class that computes an order total and logs its progress through an injected `Logger` reference:

* `Logger` — abstract base with a pure-virtual `info(const std::string&)` method.
* `ConsoleLogger` — production adapter that prints `INFO: <msg>` to `stdout`.
* `OrderProcessor(Logger&)` — accepts any `Logger` by reference.
* `process_order(items)` — sums `quantity * unit_price` for each `LineItem`, logs a "Processing…" message and the final total, then returns the total.

Tests use a **VectorLogger** (defined only in `tests.cpp`) that captures messages into a `std::vector<std::string>`, proving the processor works without any console I/O.

## How to Run

```bash
cd day37/solution
make          # build + run tests
make asan     # AddressSanitizer build
make valgrind # Valgrind memory check
make clean    # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand DIP, ports and adapters, and constructor injection.
2. Study the abstract `Logger` interface in `exercise.hpp`.
3. Implement `ConsoleLogger::info` and `OrderProcessor::process_order` in `exercise.cpp`.
4. Run `make` and iterate until all assertions pass with zero warnings.
5. Review `materials/links.md` for deeper reading.

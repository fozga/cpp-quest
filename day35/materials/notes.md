# Putting It All Together — Notes

## A. Integrating Refactoring, Tests, and Tools

The three pillars of sustainable code quality reinforce each other:

| Pillar | Role | Without it |
|--------|------|------------|
| **Refactoring** | Keeps the codebase simple and expressive. | Complexity accumulates; every change is risky. |
| **Unit tests** | Prove that refactoring preserved behaviour. | You refactor blind — regressions hide until production. |
| **Static analysis** | Catches defects that tests may not exercise. | Subtle bugs (signed overflow, use-after-move) slip through. |

### The Virtuous Cycle

1. **Write a test** that specifies the behaviour you need.
2. **Implement** the simplest code that passes the test.
3. **Refactor** to improve clarity, remove duplication, and satisfy analysis.
4. **Run tools** (compiler warnings, sanitizers, clang-tidy) to catch what tests
   missed.
5. Repeat.

Each step makes the next one easier. Tests give you confidence to refactor;
clean code is easier to analyse; analysis findings guide targeted tests.

## B. Design Discipline

### Clear Ownership

Every object should have exactly one owner responsible for its lifetime. In
modern C++ this means:

- **Value semantics** for small, copyable types.
- **`std::unique_ptr`** when heap allocation is necessary and ownership is
  exclusive.
- **References / `std::string_view`** for non-owning access.

Ambiguous ownership leads to leaks, double-frees, and data races — all of which
sanitizers and static analysis flag.

### Small, Composable Functions

A function should do **one thing** and do it well. Benefits:

- Easier to name (a clear name = a clear responsibility).
- Easier to test (fewer inputs, fewer branches).
- Easier to reuse (composition over monoliths).

If a function exceeds ~20 lines or requires more than a short paragraph to
describe, consider splitting it.

### Avoid Premature Generalisation

Resist the urge to build frameworks. Start with a concrete implementation that
solves the immediate problem. Generalise only when you have **at least two
concrete use cases** and the duplication is clear. Premature abstraction adds
indirection, increases cognitive load, and makes analysis harder.

### Error Handling Strategy

Choose a consistent strategy and stick with it:

| Mechanism | Use when |
|-----------|----------|
| **Exceptions** | The caller cannot reasonably be expected to check a precondition (e.g. missing map key). |
| **Return codes / `std::optional`** | Failure is a normal, expected outcome (e.g. parsing user input). |
| **`assert` / contracts** | Conditions that indicate programmer error, not runtime failure. |

Mixing strategies arbitrarily confuses both human readers and tools.

## Self-Check Questions

1. How do refactoring, testing, and static analysis reinforce each other?
2. What happens if you refactor without tests?
3. Why is clear ownership important for both correctness and tooling?
4. When should you use `std::unique_ptr` versus a plain value?
5. What is the benefit of keeping functions small and focused?
6. What does "premature generalisation" mean, and why is it harmful?
7. When is an exception the right error-handling mechanism versus returning
   `std::optional`?
8. How do sanitizers (ASan, UBSan) complement unit tests?
9. Why should you mark member functions `const` when they do not modify state?
10. Describe the "virtuous cycle" of test → implement → refactor → analyse.

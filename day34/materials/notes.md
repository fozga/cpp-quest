# Static Analysis & Code Style — Notes

## A. Static Analysis: Examining Code Without Running It

Static analysis inspects source code (or its AST) to find defects, style
violations, and suspicious patterns **at build time** — before any test or user
ever exercises the path.

### Popular C++ Tools

| Tool | Strength |
|------|----------|
| **clang-tidy** | Hundreds of checks covering modernisation, readability, performance, and bug-prone patterns. Integrates with CMake and most editors. |
| **cppcheck** | Lightweight, zero–false-positive philosophy. Good at catching null dereferences, buffer overruns, and resource leaks. |
| **SonarQube / SonarCloud** | Enterprise-grade dashboard that aggregates findings across an entire project with trend tracking. |
| **PVS-Studio** | Deep inter-procedural analysis; particularly strong on copy-paste and integer-overflow bugs. |

### Categories of Findings

- **Bug risks** — null dereference, use-after-free, signed/unsigned mismatch.
- **Undefined behaviour** — shifting by negative amount, modifying a variable
  twice between sequence points.
- **Style / readability** — inconsistent naming, overly complex expressions,
  missing `const`.
- **Performance** — unnecessary copies, repeated lookups, allocations inside
  loops.
- **Dead code** — unreachable branches, unused variables, includes that resolve
  to nothing.

## B. Writing Analysis-Friendly Code

Code that passes strict analysis is almost always **easier to read and maintain**.
The following practices satisfy both tools and human reviewers:

### RAII Everywhere
Acquire resources in constructors, release in destructors. Tools flag raw
`new`/`delete` and unmatched `fopen`/`fclose` as high-severity findings.

### Const by Default
Mark every variable, parameter, and member function `const` unless mutation is
required. This eliminates an entire class of accidental-modification bugs and
gives the analyser (and the reader) a clear signal about intent.

### Avoid Preprocessor Macros
Macros bypass the type system and confuse analysis tools. Prefer `constexpr`
variables, `inline` functions, and `enum class` for the same goals.

### Eliminate Dead Code
Unused variables trigger `-Wunused`; unreachable branches trigger
`-Wunreachable-code`. Removing them keeps the signal-to-noise ratio high.

### Clear Naming
`format_timestamp` beats `fmt_ts`. Descriptive names reduce cognitive load and
make tool diagnostics easier to map back to intent.

### Small, Pure Functions
Functions with no side effects and clear input → output contracts are trivially
testable and trivially analysable.

## Self-Check Questions

1. What does "static analysis" mean, and how does it differ from testing?
2. Name two popular static-analysis tools for C++ and one strength of each.
3. What categories of defects can static analysis catch that a compiler's default
   warnings might miss?
4. Why does RAII help static analysis tools reason about resource lifetimes?
5. How does `const`-correctness reduce the surface area for bugs?
6. Why are preprocessor macros problematic for static analysis?
7. What compiler flags enable warnings about unused variables and unreachable
   code?
8. Give an example of "dead code" that a tool would flag but a human might
   overlook.
9. How does clear naming improve the usefulness of diagnostic messages?
10. Why are small, pure functions easier for both humans and tools to analyse?

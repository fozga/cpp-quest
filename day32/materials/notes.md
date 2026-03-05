# Unit Testing Mindset — Notes

## A. Purpose of Unit Tests

Unit tests serve two critical roles:

1. **Catch regressions** — When you change code, passing tests confirm that
   existing behaviour is preserved. A failing test immediately tells you *what*
   broke and *where*.
2. **Living documentation** — Tests describe how functions are intended to
   behave. Unlike comments, tests are executable and stay in sync with the code
   (or loudly fail when they don't).

Tests also drive better design: writing a test first (or alongside the code)
forces you to think about the public interface, inputs, outputs, and edge cases.

## B. The AAA Pattern

Every well-structured test follows three phases:

| Phase | Purpose | Example |
|-------|---------|---------|
| **Arrange** | Set up inputs and expected values. | `std::string input = "  hello  ";` |
| **Act** | Call the function under test — **one** call per test. | `auto result = trim(input);` |
| **Assert** | Verify the result matches expectations. | `assert(result == "hello");` |

Keeping exactly **one Act** per test ensures that a failure points directly to
the behaviour that broke. If a test exercises two actions, a failure is ambiguous.

## C. Qualities of Good Tests

| Quality | What it means |
|---------|---------------|
| **Isolated** | No test depends on the result or side-effects of another test. Each test sets up its own state. |
| **Fast** | Unit tests should run in milliseconds so you execute them after every small change. |
| **Deterministic** | The same inputs always produce the same result. No reliance on wall-clock time, network, or random values. |
| **Focused** | Each test verifies a single behaviour or edge case. This makes the test name meaningful and failures easy to diagnose. |
| **Clearly named** | A reader can understand the scenario and expected outcome from the test name or description alone. |

### Common Test Smells

- **Multiple asserts on unrelated behaviours** — Split into separate tests.
- **Test depends on execution order** — Introduce proper setup in each test.
- **Hidden global state** — Pass dependencies explicitly.
- **Asserting implementation details** — Test observable behaviour, not internal
  mechanics.

## Self-Check Questions

1. What are the three phases of the AAA pattern, and what happens in each?
2. Why should a test contain only one Act (one call to the function under test)?
3. How do unit tests act as "living documentation"?
4. What does it mean for a test to be *deterministic*?
5. Why is test isolation important — what can go wrong without it?
6. Give an example of a "test smell" involving multiple asserts.
7. Why must you cast to `unsigned char` before passing a `char` to `std::isspace`
   or `std::toupper`?
8. How does writing tests *before* implementation (TDD) influence interface
   design?
9. What is the risk of asserting implementation details rather than observable
   behaviour?
10. If a test is slow or flaky, what qualities from the list above does it
    violate, and how would you fix it?

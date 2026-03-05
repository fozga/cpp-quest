# Refactoring Safely — Notes

## A. What Is Legacy Code?

Michael Feathers offers a pragmatic definition in *Working Effectively with
Legacy Code*: **legacy code is code without tests**. Without an automated test
suite, every change is a leap of faith — you cannot quickly verify that existing
behavior is preserved.

Symptoms of legacy code include:

- **Long functions** that mix multiple responsibilities (computation, I/O,
  error handling) in a single block.
- **Hidden data flow** — intermediate results stored in local variables that
  are read far from where they are written, making it hard to trace which
  values depend on which.
- **Implicit output dependencies** — functions that print directly to
  `std::cout` or write to files, making them impossible to test without
  redirecting global streams.
- **No seams for testing** — because everything is tangled together, there is
  no way to call the computation in isolation and inspect its result.
- **Fear of change** — developers avoid touching the code because they have
  no way to tell whether a modification broke something.

## B. Safe Refactoring

Refactoring means changing the internal structure of code without altering its
externally observable behavior. Safe refactoring follows a disciplined process:

1. **Write characterization tests first.** Before touching any logic, capture
   the current behavior in automated tests. These tests do not assert that the
   code is *correct* — they assert that it does *what it currently does*. Any
   deviation after refactoring signals a regression.

2. **Make small, incremental changes.** Each refactoring step should be tiny
   enough that if something breaks, you know exactly which change caused it.
   Resist the urge to restructure everything at once.

3. **Apply mechanical refactorings.** Well-known, behavior-preserving
   transformations — Extract Function, Introduce Parameter Object, Rename
   Variable — are less error-prone than ad-hoc restructuring because they
   follow a fixed recipe.

4. **Run the full test suite after every step.** The tests are your safety
   net. If they pass after a change, you have high confidence the behavior is
   intact. If they fail, you revert the last change and try a smaller step.

5. **Keep the old interface alive as a façade.** If other code depends on the
   original function signature, wrap the new implementation behind the old
   name. This lets you improve internals without forcing callers to change.

## C. Separating Computation from I/O

The single most impactful refactoring for testability is splitting a function
that *computes and prints* into two parts:

- **A pure computation function** that takes inputs and returns a result,
  with no side effects. It does not read from `std::cin`, write to
  `std::cout`, open files, or modify global state.
- **An I/O wrapper** that takes the computed result and handles formatting
  and output.

Why this matters:

- **Pure functions are trivially testable.** Call the function, inspect the
  return value. No mocks, no stream redirection, no temporary files.
- **Pure functions are composable.** The result can be passed to different
  formatters (text, JSON, HTML) without re-running the computation.
- **I/O wrappers accept a `std::ostream&`** so tests can pass a
  `std::ostringstream` and inspect the formatted output as a string.
- **The separation clarifies intent.** Readers immediately see which part
  does the work and which part presents the result.

The façade pattern ties them together: the original function signature is
preserved, but its body simply calls the pure function and passes the result
to the I/O function. Existing callers never need to change.

## D. Self-Check Questions

1. What is the pragmatic definition of legacy code given by Michael Feathers?
2. Why should you write characterization tests *before* refactoring, rather
   than after?
3. What does it mean for a refactoring to be "behavior-preserving"?
4. Why are small, incremental refactoring steps safer than large rewrites?
5. What is a "pure function" and why is it easier to test than a function
   that performs I/O?
6. How does accepting a `std::ostream&` parameter make an output function
   testable?
7. What is the purpose of a façade function after refactoring?
8. Name two mechanical refactorings discussed in this lesson and describe
   what each one does.
9. If a characterization test fails after a refactoring step, what should
   you do?
10. Why is mixing computation with I/O in a single function considered a
    code smell?

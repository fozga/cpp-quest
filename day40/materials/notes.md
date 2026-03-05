# Final Project – Implementation — Notes

## A. Completing a Project

Finishing a project well means more than just making it compile:

* **Clear invariants** — Document (or enforce in code) the rules that must always hold. For `TaskList`, one invariant is that every task has a unique, positive `id`.
* **Small functions** — Each function should do one thing. If a function is hard to test, it is probably doing too much.
* **Consistent error reporting** — Decide on a single strategy. In this project, lookup failure returns `nullptr` and removal failure returns `false`. Mixing exceptions, error codes, and silent failures in the same API leads to bugs.
* **No dead code** — Every declared function should have a definition, and every definition should be reachable from at least one test.

## B. Testing and Confidence

A comprehensive test suite covers three categories:

| Category | Description | Example |
|----------|-------------|---------|
| **Normal usage** | The expected, happy path. | Add a task, find it by ID, verify fields. |
| **Edge cases** | Boundary conditions and empty states. | Call `find_task` on an empty list; remove a task that was already removed. |
| **State transitions** | Operations that change the collection and affect later operations. | Add three tasks, remove the middle one, verify `list_all` returns exactly two. |

Tips for effective testing:

* **One assertion per logical check** — If an assertion fails, you should immediately know *which* property was violated.
* **Print progress** — A `[PASS]` message after each group of assertions lets you see how far the suite got before a failure.
* **Test independence** — Where practical, create a fresh `TaskList` for each group so earlier mutations do not mask later bugs.
* **Sanitizer runs** — Compile with `-fsanitize=address` at least once per project. It catches use-after-free, buffer overflows, and leaks that silent assertions will miss.

## C. Self-Check Questions

1. What is an *invariant* in the context of a class like `TaskList`?
2. Why does `find_task` return a pointer instead of a reference?
3. What happens if you store a `Task&` returned by `add_task` and then call `add_task` again — is the reference still valid? Why or why not?
4. How does returning `bool` from `remove_task` help callers handle the "not found" case?
5. Why is `list_by_status` a query (returns a copy) rather than returning references into the internal vector?
6. What is the purpose of running tests under AddressSanitizer?
7. Name two edge cases worth testing for an `add_task` / `remove_task` cycle.
8. Why should `size()` be `noexcept`?
9. If you wanted to make `Task` comparable with `==`, what fields would you compare?
10. How would you extend the API to support updating a task's status — what signature would you choose?

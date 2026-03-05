# Day 39 — Notes: API-First Design

## A. API-First Design

Designing the **public interface** before writing implementation code is one of the most effective techniques for producing clean, maintainable software.

Key benefits:

* **Clarity of responsibility** — Writing function signatures forces you to decide what each component is responsible for and what it is *not*.
* **Easier testing** — When the API exists as a header, you can write tests immediately, even before the bodies are filled in.
* **Reduced churn** — Discovering a missing operation at the design stage costs minutes; discovering it after 500 lines of implementation can cost hours.
* **Communication** — A header file serves as living documentation of the module's capabilities.

A good API-first workflow looks like:

1. List the **operations** the module must support.
2. Choose **parameter types** and **return types** that make the common case easy and the error case explicit.
3. Write the header (declarations only).
4. Compile to check for syntax errors.
5. Write a test file that exercises the API.
6. Implement the bodies, running tests after each function.

## B. Entities and Relationships

In this project the domain has two main abstractions:

* **Task** — a plain value type (a *struct*). It holds data: an id, a title, a status, and an optional due date. Because it is a value type, copies are cheap and comparison is field-by-field.
* **TaskList** — a *collection* that owns a vector of `Task` objects. It is the single point of mutation: only `TaskList` assigns IDs and only `TaskList` can add or remove tasks.

Operations fall into two categories:

| Category | Examples | Characteristics |
|----------|----------|-----------------|
| **Queries** | `find_task`, `list_all`, `list_by_status`, `size` | Do not modify state; should be `const`. |
| **Mutations** | `add_task`, `remove_task` | Change internal state; may invalidate pointers. |

Keeping queries `const` and mutations non-`const` makes the API self-documenting: callers holding a `const TaskList&` know they cannot accidentally change data.

## C. Self-Check Questions

1. What does "API-first design" mean in practice?
2. Why is it useful to compile a header before writing any function bodies?
3. What is the difference between a *query* and a *mutation* on a collection class?
4. Why should query methods be marked `const`?
5. What advantage does returning `Task*` (pointer) have over returning `Task&` (reference) for a lookup that might fail?
6. Why does `add_task` return a `Task&` rather than a copy?
7. How does `std::optional<std::string>` improve the `due_date` field compared to using an empty string as a sentinel?
8. What risk exists when storing a reference or pointer returned by `add_task` and later calling `add_task` again?
9. Why is `size()` marked `noexcept`?
10. In what order would you implement the functions if you were writing them one at a time, running tests after each?

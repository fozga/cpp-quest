# Project Layout — Notes

## A. Typical C++ Project Layout

Most non-trivial C++ projects converge on a directory structure that separates concerns by file purpose:

```
project/
├── include/          # public headers (installed / consumed by clients)
│   └── project/
│       ├── core.hpp
│       └── util.hpp
├── src/              # implementation files (.cpp) and private headers
│   ├── core.cpp
│   └── util.cpp
├── tests/            # test sources and fixtures
│   └── test_core.cpp
├── CMakeLists.txt    # or Makefile, Meson, etc.
└── README.md
```

Key conventions:

* **`include/`** holds headers that define the public API. A nested subdirectory matching the project name (`include/project/`) prevents header-name collisions when the library is installed alongside others.
* **`src/`** contains `.cpp` files and any headers that are internal to the implementation. Nothing outside `src/` should `#include` a private header.
* **`tests/`** keeps test code separate from production code. Each test file typically corresponds to one module or feature.
* Per-module subdirectories within `src/` and `include/` (e.g., `src/core/`, `src/util/`) are common in larger projects. Each subdirectory groups closely related files.

Keeping this separation consistent means build scripts can apply different compile flags to `src/` vs `tests/`, install only `include/` for downstream consumers, and package release artifacts without test infrastructure.

## B. Mapping Modules to Directories

A **module** in layout terms is a cohesive unit of functionality — a set of closely related types and functions. The mapping rule is simple:

* Each module gets **its own header** (and usually its own `.cpp`).
* Headers declare only what the module's clients need; implementation details stay in the source.
* Cross-module dependencies should be **minimized and explicit**. If module A includes module B's header, that dependency is visible and intentional.

Signs that module boundaries are drawn well:

* Removing a module requires changes only to the modules that explicitly depend on it.
* Each header compiles on its own without relying on a particular include order.
* Internal helpers are in anonymous namespaces or `static` functions within the `.cpp`, invisible to the rest of the project.

When a single header or source file grows beyond a few hundred lines, it is often a sign that two distinct modules have been combined and should be split.

## C. Self-Check Questions

1. What are the three top-level directories in a conventional C++ project layout, and what does each contain?
2. Why is a nested subdirectory (e.g., `include/project/`) used inside the `include/` folder?
3. What distinguishes a header placed in `include/` from a header placed in `src/`?
4. How does separating `tests/` from `src/` benefit build configuration?
5. What is the relationship between a logical module and its physical files?
6. Why should each header be compilable on its own, without depending on a particular include order?
7. How do anonymous namespaces or `static` functions in a `.cpp` file support module encapsulation?
8. What signals suggest a single source file should be split into two modules?
9. How does minimizing cross-module `#include` directives reduce coupling?
10. In a project with `core`, `util`, and `app` modules, which module(s) would you expect to depend on the others, and why?

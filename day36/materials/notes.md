# Library Boundaries — Notes

## A. Public API vs Internal Implementation

A library's **public API** is the set of declarations visible to consumers: class definitions in headers, free-function signatures, type aliases, and constants. Everything else — helper functions, internal data structures, algorithm details — is **implementation**.

Keeping the boundary explicit reduces **coupling**: clients depend on a contract, not on how that contract is fulfilled. When implementation changes, no consumer code needs recompilation, and ABI stability becomes achievable.

A useful rule of thumb: if removing or changing a symbol would break client code, it is part of the public API; otherwise it belongs behind the boundary.

## B. Information Hiding

### Minimal Headers

Headers should include only what is strictly required for the declarations they contain. Every extra `#include` widens the dependency surface and slows compilation.

Prefer **forward declarations** over includes when a type appears only as a pointer, reference, or in a function signature (not by value). This is the foundation of the **pImpl idiom** (pointer to implementation).

### The pImpl Idiom

pImpl hides a class's data members and private helpers behind a forward-declared struct:

```cpp
// widget.hpp
#pragma once
#include <memory>

class Widget {
public:
    Widget();
    ~Widget();
    void do_work();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
```

```cpp
// widget.cpp
#include "widget.hpp"
#include <vector>  // only needed here

struct Widget::Impl {
    std::vector<int> data;
};

Widget::Widget() : impl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;

void Widget::do_work() { impl_->data.push_back(42); }
```

Key points:

* The destructor **must** be defined in the `.cpp` where `Impl` is a complete type; otherwise `unique_ptr` cannot call `delete`.
* Move operations must also be defined (or defaulted) in the `.cpp` for the same reason.
* Copy is typically deleted unless you explicitly deep-copy the `Impl`.

### Benefits of pImpl

* Client code never sees internal includes (`<vector>`, `<unordered_map>`, etc.).
* Changing `Impl` fields is a source-only change — no header change, no recompile cascade.
* Compile-time firewall: heavy headers stay out of the public interface.

## C. Headers and Sources — What Goes Where

| Belongs in the **header** | Belongs in the **source** |
|---|---|
| Class declaration with public/protected members | `Impl` struct definition |
| Forward declarations of implementation types | Member function bodies |
| Inline/constexpr functions (if needed by clients) | Internal helper functions |
| Type aliases that are part of the API | Includes for implementation-only types |
| Include guards / `#pragma once` | Static or anonymous-namespace utilities |

A good test: open the header and ask "does a consumer need this line?" If not, move it to the source.

## D. Self-Check Questions

1. What is the difference between a library's public API and its implementation?
2. Why does reducing the number of `#include` directives in a header matter?
3. When can you use a forward declaration instead of an `#include`?
4. In the pImpl idiom, why must the destructor be defined in the `.cpp` file rather than in the header?
5. Why are move operations typically defaulted in the `.cpp` instead of the header when using pImpl with `unique_ptr`?
6. If you add a new private data member to an `Impl` struct, which files need recompilation?
7. What happens if a header exposes an `#include <unordered_map>` that only the implementation needs?
8. How does pImpl affect a class's copy semantics by default?
9. What role do anonymous namespaces play in limiting symbol visibility within a translation unit?
10. Name two trade-offs of using the pImpl idiom compared to a plain class definition.

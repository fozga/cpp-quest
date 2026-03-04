# Move Constructors — Links & Resources

Supplementary reading on move semantics, rvalue references, and related topics.

- [cppreference — Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)
  Authoritative reference for the move constructor's declaration, behaviour,
  implicit generation rules, and interaction with `noexcept`.

- [cppreference — std::move](https://en.cppreference.com/w/cpp/utility/move)
  Explains what `std::move` does (an unconditional cast to rvalue reference)
  and when to use it.

- [Thomas Becker — C++ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_01.html)
  A thorough, example-driven walkthrough of rvalue references, move semantics,
  and perfect forwarding.

- [Howard Hinnant — Everything about Move Semantics (video)](https://www.youtube.com/watch?v=vLinb2fgkHk)
  Talk by the original designer of C++ move semantics covering design
  rationale and best practices.

- [Effective Modern C++, Item 29 — Assume move operations are not present, not cheap, and not used](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)
  Scott Meyers discusses when and why moves may not happen and how to ensure
  your types benefit from them.

- [C++ Core Guidelines — C.64: A move operation should move and leave its source in a valid state](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-move-semantic)
  Guideline on post-move validity and the broader set of resource-management
  rules.

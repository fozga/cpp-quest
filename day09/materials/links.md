# Day 09 — Further Reading

The resources below provide deeper coverage of the `override` and `final`
specifiers and their role in safe inheritance design.

- **cppreference — override specifier**
  https://en.cppreference.com/w/cpp/language/override
  Formal reference for `override` syntax, compiler checks, and interaction
  with covariant return types.

- **cppreference — final specifier**
  https://en.cppreference.com/w/cpp/language/final
  Reference page covering `final` on both virtual functions and class
  definitions, with examples.

- **cppreference — Virtual function specifier**
  https://en.cppreference.com/w/cpp/language/virtual
  Comprehensive page on virtual functions, vtables, pure virtuals, and how
  `override` / `final` fit into the dispatch model.

- **C++ Core Guidelines — C.128: Use `override`**
  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c128-virtual-functions-should-specify-exactly-one-of-virtual-override-or-final
  Guideline recommending that every virtual function specifies exactly one
  of `virtual`, `override`, or `final`, with rationale and enforcement
  notes.

- **Arthur O'Dwyer — "When should you use `final`?"**
  https://quuxplusone.github.io/blog/2021/02/15/devirtualization/
  Blog post exploring the devirtualisation benefits of `final` and when the
  performance argument actually matters in practice.

- **Microsoft Learn — `override` and `final` (C++)**
  https://learn.microsoft.com/en-us/cpp/cpp/override-specifier
  Tutorial-style explanation with annotated examples covering both
  specifiers in MSVC.

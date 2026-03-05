# Small Buffer Optimization — Links

Supplementary reading on SBO, placement `new`, and custom `any` implementations.

- [CppReference — Placement new](https://en.cppreference.com/w/cpp/language/new#Placement_new)
  Reference documentation for placement `new` syntax and semantics.

- [CppReference — std::any](https://en.cppreference.com/w/cpp/utility/any)
  The standard `any` type, which uses SBO in most implementations.

- [CppReference — alignas / alignof](https://en.cppreference.com/w/cpp/language/alignas)
  Language reference for alignment specifiers and queries.

- [Arthur O'Dwyer — "The Rule of SBO"](https://quuxplusone.github.io/blog/2019/08/02/the-rule-of-sbo/)
  Blog post discussing the design implications and trade-offs of SBO.

- [Implementing std::any from scratch (David Mazières)](https://www.scs.stanford.edu/~dm/blog/va-opt.html)
  A walkthrough of building a type-erased container step by step.

- [Abseil Tip #148 — Small Buffer Optimization](https://abseil.io/tips/148)
  Google's guidance on when SBO is worthwhile and when it adds unnecessary
  complexity.

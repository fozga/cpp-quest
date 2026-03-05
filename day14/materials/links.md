# Exception Removal — Links

Curated references on `std::expected`, error handling strategies, and the rationale for
moving away from exceptions in performance-sensitive C++.

- [std::expected — cppreference](https://en.cppreference.com/w/cpp/utility/expected)
  Comprehensive reference for the C++23 `std::expected` class template.

- [P0323R12 — std::expected proposal](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0323r12.html)
  The final WG21 proposal that introduced `std::expected` into the standard.

- [Herb Sutter — De-fragmenting C++: Making Exceptions More Affordable and Usable](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0709r4.pdf)
  Explores the cost of exceptions and proposes lighter-weight alternatives.

- [Sy Brand — Functional Error Handling with std::expected](https://devblogs.microsoft.com/cppblog/cpp23-std-expected/)
  Practical guide to monadic error handling with `std::expected` in C++23.

- [Jason Turner — C++ Weekly: std::expected](https://www.youtube.com/watch?v=PH4WBuE1BHI)
  Short video walkthrough of `std::expected` usage and benefits.

- [Andrei Alexandrescu — Systematic Error Handling in C++](https://www.youtube.com/watch?v=kaI4R0Ng4E8)
  Classic talk on discriminated-union error types that influenced the design of `expected`.

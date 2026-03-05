# Day 06 — Further Reading

The resources below provide deeper coverage of copy elision, RVO, NRVO,
and their interaction with move semantics. All links point to
well-maintained references and articles.

- **cppreference — Copy Elision**
  https://en.cppreference.com/w/cpp/language/copy_elision
  Comprehensive reference page covering all forms of copy elision including
  mandatory elision in C++17, NRVO, and the conditions under which the
  compiler is permitted or required to omit copies and moves.

- **cppreference — Value Categories (prvalue, xvalue, lvalue)**
  https://en.cppreference.com/w/cpp/language/value_category
  Understanding value categories is essential for reasoning about when
  mandatory elision applies (prvalues) versus when NRVO is merely
  permitted (lvalues).

- **Arthur O'Dwyer — "Return Value Optimization: Harder Than It Looks"**
  https://quuxplusone.github.io/blog/2018/03/29/the-superconstructing-super-elider/
  An in-depth blog post exploring edge cases of RVO and NRVO, including
  situations where the compiler cannot apply the optimization.

- **Abseil Tip #11 — Return Policy**
  https://abseil.io/tips/11
  Practical guidance from the Abseil C++ Tips series on returning objects
  by value, when to rely on copy elision, and common mistakes that
  pessimize returns.

- **Microsoft Learn — Copy Elision and Return Value Optimization**
  https://learn.microsoft.com/en-us/cpp/cpp/copy-elision
  Tutorial-style explanation of how MSVC handles RVO and NRVO with
  examples showing when elision occurs and when it does not.

- **Jason Turner — C++ Weekly: Understanding RVO**
  https://www.youtube.com/watch?v=AahL7MnLsKA
  Short video walkthrough demonstrating RVO and NRVO with live compiler
  output, showing exactly when copies and moves are elided.

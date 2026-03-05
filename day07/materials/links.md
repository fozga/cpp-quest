# Perfect Forwarding — Links

Curated references for understanding forwarding references, `std::forward`, and
related template argument deduction rules.

- [cppreference — std::forward](https://en.cppreference.com/w/cpp/utility/forward)
  Authoritative reference for `std::forward` semantics, including the
  distinction between the two overloads.

- [cppreference — Reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing)
  Formal rules for how `T& &&` and similar combinations resolve.

- [Scott Meyers — Universal References in C++11](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)
  The original article coining "universal references" and explaining the
  deduction rules with clear examples.

- [Thomas Becker — C++ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_01.html)
  Comprehensive walkthrough of rvalue references, move semantics, and perfect
  forwarding with diagrams.

- [Jason Turner — C++ Weekly: Perfect Forwarding](https://www.youtube.com/watch?v=pIzaZbKUw2s)
  Short video explaining the mechanics and common mistakes of perfect
  forwarding.

- [Eli Bendersky — Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/)
  Blog post with annotated code samples walking through reference collapsing
  and `std::forward` step by step.

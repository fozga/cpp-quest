# Circular References — Further Reading

The resources below cover `shared_ptr`, `weak_ptr`, and the circular-reference
problem in depth. Skim at least two before moving on.

- **cppreference — `std::shared_ptr`**
  <https://en.cppreference.com/w/cpp/memory/shared_ptr>
  Comprehensive reference for `shared_ptr` API, control-block semantics, and
  thread-safety guarantees.

- **cppreference — `std::weak_ptr`**
  <https://en.cppreference.com/w/cpp/memory/weak_ptr>
  Full API documentation including `lock()`, `expired()`, and interaction with
  `shared_ptr`.

- **Herb Sutter — "Leak-Freedom in C++… By Default"** (CppCon 2016)
  <https://www.youtube.com/watch?v=JfmTagWcqoE>
  Talk covering ownership graphs, cycles, and the role of `weak_ptr` in
  achieving leak-free designs.

- **Bartlomiej Filipek — "Smart Pointers Gotchas"**
  <https://www.cppstories.com/2021/smart-ptr-gotchas/>
  Practical article on common pitfalls with `shared_ptr`, including accidental
  cycles and dangling `weak_ptr`.

- **ISO C++ FAQ — "How do I avoid memory leaks with shared_ptr cycles?"**
  <https://isocpp.org/wiki/faq/cpp11-library#shared-ptr-prevent-cycles>
  Short FAQ entry explaining the cycle problem and the `weak_ptr` solution.

- **Microsoft Learn — Smart Pointers (Modern C++)**
  <https://learn.microsoft.com/en-us/cpp/cpp/smart-pointers-modern-cpp>
  Overview of `unique_ptr`, `shared_ptr`, and `weak_ptr` with ownership
  diagrams and best-practice guidance.

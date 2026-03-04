# Vtables & Memory Leaks — Further Reading

Curated references to deepen your understanding of vtables, virtual
destructors, and the memory leaks that result from getting them wrong.

- **C++ Virtual Functions and Vtables (Pablo Arias)**
  https://pabloariasal.github.io/2017/06/10/understanding-virtual-tables/
  Clear walkthrough of how vtables and vptrs work under the hood with diagrams.

- **Virtual Destructor — cppreference.com**
  https://en.cppreference.com/w/cpp/language/destructor#Virtual_destructors
  Authoritative reference on virtual destructor semantics and requirements.

- **C++ Core Guidelines C.35 — A base class destructor should be either public
  and virtual, or protected and non-virtual**
  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-virtual
  The official guideline with rationale and examples.

- **Back to Basics: Virtual Dispatch — CppCon (Klaus Iglberger)**
  https://www.youtube.com/watch?v=wHnQ4CL0Clk
  Conference talk covering virtual dispatch mechanics, performance
  implications, and common pitfalls.

- **Valgrind Quick Start Guide**
  https://valgrind.org/docs/manual/quick-start.html
  How to use Valgrind's Memcheck to detect leaks caused by missing virtual
  destructors.

- **AddressSanitizer — Clang Documentation**
  https://clang.llvm.org/docs/AddressSanitizer.html
  Reference for ASan flags and interpreting leak reports.

# Debugging with Tools — Links

Curated resources for understanding AddressSanitizer, Valgrind, and memory
debugging in C++.

- **AddressSanitizer — Clang Documentation**
  <https://clang.llvm.org/docs/AddressSanitizer.html>
  Official reference for ASan flags, supported checks, and suppression files.

- **Google Sanitizers Wiki (GitHub)**
  <https://github.com/google/sanitizers/wiki/AddressSanitizer>
  In-depth explanations of ASan internals, shadow memory layout, and FAQ.

- **Valgrind Quick Start Guide**
  <https://valgrind.org/docs/manual/quick-start.html>
  Minimal walkthrough to get Valgrind running on your first program.

- **Valgrind Memcheck Manual**
  <https://valgrind.org/docs/manual/mc-manual.html>
  Full documentation for Memcheck — error types, suppression files, and options.

- **CppReference — RAII**
  <https://en.cppreference.com/w/cpp/language/raii>
  Background on the ownership pattern that prevents most manual memory bugs.

- **Sy Brand — "How to Use Sanitizers"**
  <https://devblogs.microsoft.com/cppblog/addresssanitizer-asan-for-windows-with-msvc/>
  Practical guide covering ASan on multiple platforms including MSVC.

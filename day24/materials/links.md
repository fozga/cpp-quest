# Modern Threads — Links

Curated resources for `std::jthread` and `std::stop_token` in C++20.

- [std::jthread — cppreference.com](https://en.cppreference.com/w/cpp/thread/jthread)
  Authoritative API reference for `jthread`, including constructor overloads and stop-token wiring.

- [std::stop_token — cppreference.com](https://en.cppreference.com/w/cpp/thread/stop_token)
  Full documentation of the cooperative cancellation primitives: `stop_token`, `stop_source`, and `stop_callback`.

- [A Tour of C++ Concurrency — Nicolai Josuttis (jthread proposal author)](https://www.josuttis.com/c++20/jthread.html)
  Motivation and design rationale from the original proposal author.

- [C++20 jthread and stop_token — Modernes C++](https://www.modernescpp.com/index.php/a-new-thread-with-c-20-std-jthread/)
  Beginner-friendly walkthrough with examples comparing `std::thread` and `std::jthread`.

- [P0660R10 — Stop Token and Joining Thread](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0660r10.pdf)
  The accepted ISO proposal. Read sections 1-3 for motivation and design decisions.

# Deadlock Prevention — Links

Curated resources for understanding deadlocks and `std::scoped_lock` in C++.

- [std::scoped_lock — cppreference.com](https://en.cppreference.com/w/cpp/thread/scoped_lock)
  Full API reference including constructor behaviour, deduction guides, and exception guarantees.

- [std::lock — cppreference.com](https://en.cppreference.com/w/cpp/thread/lock)
  The underlying deadlock-avoidance algorithm that `scoped_lock` uses internally.

- [Deadlock — Wikipedia](https://en.wikipedia.org/wiki/Deadlock)
  Clear explanation of the four Coffman conditions with diagrams and real-world analogies.

- [C++17 scoped_lock — Modernes C++](https://www.modernescpp.com/index.php/scoped-lock/)
  Beginner-friendly walkthrough comparing `lock_guard`, `unique_lock`, and `scoped_lock`.

- [CppCon 2017: Fedor Pikus — Read-Write Locking (YouTube)](https://www.youtube.com/watch?v=lkgszkPnV8g)
  Conference talk covering lock ordering, deadlock avoidance, and practical multi-mutex patterns.

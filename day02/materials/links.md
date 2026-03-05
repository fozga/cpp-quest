# Custom Deleters — Further Reading

The following resources cover `std::unique_ptr`, custom deleters, and related
RAII patterns in depth.

- **cppreference — `std::unique_ptr`**
  https://en.cppreference.com/w/cpp/memory/unique_ptr
  Comprehensive reference for the full `unique_ptr` API including constructor
  overloads that accept custom deleters.

- **cppreference — `std::unique_ptr<T,Deleter>::get_deleter`**
  https://en.cppreference.com/w/cpp/memory/unique_ptr/get_deleter
  Documents how to retrieve and inspect the deleter stored inside a
  `unique_ptr`, useful for debugging and testing.

- **Bartlomiej Filipek — "Custom Deleters for C++ Smart Pointers"**
  https://www.cppstories.com/2016/04/custom-deleters-for-c-smart-pointers/
  Practical walkthrough of functor, lambda, and function-pointer deleters with
  size measurements and real-world examples.

- **Herb Sutter — GotW #89: Smart Pointers**
  https://herbsutter.com/2013/05/29/gotw-89-solution-smart-pointers/
  Discusses when to use `unique_ptr` vs `shared_ptr`, ownership semantics,
  and factory-function patterns — directly applicable to today's exercise.

- **Jason Turner — C++ Weekly Ep 283: Stop Using unique_ptr as "just" a Heap Pointer**
  https://www.youtube.com/watch?v=AmjoMRJVJXQ
  Short video demonstrating how `unique_ptr` with custom deleters can manage
  file handles, C library resources, and more.

- **ISO C++ Core Guidelines — R.11 and R.20**
  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-newdelete
  The official guidelines on avoiding naked `new`/`delete` and preferring
  `unique_ptr` for ownership, with rationale and enforcement notes.

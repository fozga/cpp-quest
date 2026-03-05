# SFINAE to Concepts — Links

The following references cover the migration from SFINAE to C++20 concepts.

- [Constraints and concepts — cppreference.com](https://en.cppreference.com/w/cpp/language/constraints)
  Authoritative reference for concept definitions, requires clauses, and constraint normalization.

- [std::enable_if — cppreference.com](https://en.cppreference.com/w/cpp/types/enable_if)
  Reference for the SFINAE utility being replaced, useful for side-by-side comparison.

- [From SFINAE to Concepts — Meeting C++](https://meetingcpp.com/blog/items/from-sfinae-to-concepts.html)
  Step-by-step migration guide showing how to replace `enable_if` with concepts.

- [C++20 Concepts — Practical Approach — Bartek's Coding Blog](https://www.bfilipek.com/2021/02/cpp20-concepts-practical-approach.html)
  Hands-on tutorial with practical refactoring examples.

- [How C++20 Concepts Improve Compiler Diagnostics — Modernes C++](https://www.modernescpp.com/index.php/c-20-concepts-the-details)
  Explores the diagnostic improvements concepts bring over SFINAE with real compiler output examples.

- [CppCon: SFINAE, Concepts, and the Detection Idiom (YouTube)](https://www.youtube.com/results?search_query=cppcon+sfinae+concepts+detection+idiom)
  Conference talks tracing the evolution from SFINAE patterns to concept-based designs.

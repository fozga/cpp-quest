# Day 01 — Further Reading

The resources below provide deeper coverage of RAII, the Rule of 5, and
move semantics. All links point to well-maintained references and articles.

- **cppreference — RAII**
  https://en.cppreference.com/w/cpp/language/raii
  Concise reference page defining RAII with examples of scope-based resource
  management and its interaction with exceptions.

- **cppreference — The Rule of Three/Five/Zero**
  https://en.cppreference.com/w/cpp/language/rule_of_three
  Formal description of when and why you must define or delete the special
  member functions, with code examples for each rule.

- **cppreference — Move Constructors**
  https://en.cppreference.com/w/cpp/language/move_constructor
  Reference page covering move constructor syntax, implicit generation
  rules, and the effect of `noexcept` on standard-library container
  behaviour.

- **cppreference — `std::move` and `std::exchange`**
  https://en.cppreference.com/w/cpp/utility/move
  Documents the cast utility that enables move semantics and its companion
  `std::exchange`, which is particularly handy in move operations.

- **Microsoft Learn — Move Semantics**
  https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp
  Tutorial-style walkthrough of move constructors and move assignment
  operators with annotated examples.

- **Arthur O'Dwyer — "The Rule of Zero"**
  https://quuxplusone.github.io/blog/2019/08/02/the-rule-of-zero/
  Blog post explaining why most classes should follow the Rule of 0 and
  delegate resource management to library types like `unique_ptr`.

# Library Boundaries — Links

Further reading on designing clean library interfaces, header/source separation, and the pImpl idiom.

* [C++ Core Guidelines — Interfaces (I)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-interfaces) — The official guidelines on keeping interfaces narrow, explicit, and strongly typed.
* [cppreference — pImpl Idiom](https://en.cppreference.com/w/cpp/language/pimpl) — Reference-style explanation of the pointer-to-implementation pattern with `unique_ptr`.
* [Herb Sutter — GotW #100: Compilation Firewalls](https://herbsutter.com/gotw/_100/) — Classic article on why and how to use pImpl to insulate clients from implementation changes.
* [Herb Sutter — GotW #101: Compilation Firewalls, Part 2](https://herbsutter.com/gotw/_101/) — Follow-up covering move semantics, exception safety, and performance considerations with pImpl.
* [John Lakos — Large-Scale C++ Software Design (summary notes)](https://www.aristeia.com/reviews/lakos_review.html) — Overview of physical design principles: levelization, minimal headers, and insulation techniques for large codebases.

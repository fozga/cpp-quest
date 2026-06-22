# Day 01 Task Evaluation: Pure RAII + Rule of 5

## Overall Teacher Assessment
This implementation shows a solid grasp of RAII and unique ownership. The class owns a descriptor, releases it in the destructor, prevents copying, and exposes the expected validity and access operations. The visible Day 1 tests also pass, which confirms the basic behavior is in place.

The latest revision is cleaner than the earlier versions: `reset` behaves correctly, the header is trimmed down, and move assignment now releases the current resource before taking ownership of the incoming one. That is the right ownership-transfer pattern for a handle wrapper.

The remaining issues are small and mostly stylistic. The code is functionally correct for the exercise, but some of the implementation is still a little more verbose than necessary for a small Rule-of-5 example.

**Suggested grade:** **A- / B+**. The implementation now matches the task well and behaves correctly under the provided tests.

---

## Evidence From Tests
The current implementation passes the visible Day 1 test suite when running `make day=01 test`.

That means these scenarios are working:
- default construction starts invalid
- construction from an opened pseudo file yields a valid handle
- `reset` correctly replaces a valid handle
- `reset()` correctly invalidates the handle
- move construction transfers ownership
- move assignment transfers ownership in the tested case

This is a strong signal that the class behavior matches the intended exercise.

---

## Requirement-by-Requirement Evaluation

### 1) Use `-1` as invalid sentinel
**Status:** Met

- The data member starts at `-1` in [exercise.hpp](exercise.hpp#L37).
- The move constructor invalidates the source by writing `-1` in [exercise.cpp](exercise.cpp#L44-L46).
- Move assignment also invalidates the source in [exercise.cpp](exercise.cpp#L48-L62).
- `reset` accepts `-1` as the default value in [exercise.hpp](exercise.hpp#L57).

Teacher note: This is consistent and correct.

### 2) Destructor must release the descriptor if valid and must not throw
**Status:** Met

- The destructor checks validity before closing in [exercise.cpp](exercise.cpp#L26-L42).
- It catches standard exceptions and a catch-all so exceptions do not escape [exercise.cpp](exercise.cpp#L33-L40).

Teacher note: This satisfies the non-throwing destructor requirement.

### 3) Copy constructor and copy assignment must stay deleted
**Status:** Met

- Both are explicitly deleted in [exercise.hpp](exercise.hpp#L46-L48).

Teacher note: Correct unique-ownership semantics.

### 4) Move constructor and move assignment must transfer ownership and leave the moved-from object invalid
**Status:** Met

- The move constructor transfers the descriptor and invalidates the source in [exercise.cpp](exercise.cpp#L44-L46).
- Move assignment closes the current descriptor, takes ownership of the new one, and invalidates the source in [exercise.cpp](exercise.cpp#L48-L62).

Teacher note: This is the correct resource-transfer pattern for a handle-owning type.

### 5) `is_valid()` should report whether the object currently owns a descriptor
**Status:** Met

- `is_valid()` returns true when `fd_ >= 0` in [exercise.cpp](exercise.cpp#L67-L72).

Teacher note: Correct and straightforward.

### 6) `get()` should return the raw descriptor or throw `std::runtime_error` when invalid
**Status:** Met

- Invalid access throws `std::runtime_error` in [exercise.cpp](exercise.cpp#L74-L77).
- Valid access returns the raw descriptor in [exercise.cpp](exercise.cpp#L78-L82).

Teacher note: This is exactly the required behavior.

### 7) `reset(new_handle)` should close the old descriptor if any and store `new_handle`
**Status:** Met

- `reset` closes the current descriptor when valid in [exercise.cpp](exercise.cpp#L84-L94).
- It always assigns `fd_ = new_handle` afterward in [exercise.cpp](exercise.cpp#L93-L94).

Teacher note: This is a proper reset operation and works for both valid and invalid states.

### 8) `open_pseudo_file()` and `close_pseudo_file()` should simulate open/close behavior for testing
**Status:** Met for the exercise scope

- `open_pseudo_file` produces increasing pseudo descriptors in [exercise.cpp](exercise.cpp#L9-L11).
- `close_pseudo_file` validates the descriptor range and throws on invalid close in [exercise.cpp](exercise.cpp#L13-L20).

Teacher note: This is sufficient for the teaching exercise and the provided tests.

---

## Rule-of-5 / RAII Quality Notes

### What the solution does well
- It models unique ownership correctly.
- It keeps copying disabled.
- It uses a non-throwing destructor.
- It implements `reset` correctly.
- It transfers ownership directly in move assignment, which is the right general approach.

### What could be improved
1. **The move assignment is still a little verbose**
   - It is correct, but it uses an explicit `try`/`catch` around resource release.
   - For a handle wrapper, that is acceptable, but a simpler expression would be easier to read.

2. **The move constructor and move assignment are not visually symmetric**
   - The move constructor is short and direct.
   - Move assignment must first release an existing resource, so it naturally has more steps.

---

## What I Would Tell You As Your Instructor
You have corrected the important functional issues. The class now behaves like a real RAII wrapper: it can be reset, moved, and destroyed without leaking ownership or violating the task’s semantics.

The next step is mostly about style discipline. Once ownership logic is correct, good RAII code should also be easy to read: direct, minimal, and boring in the best possible way. That usually means removing stale TODOs and trimming any remaining noise from the special members.

This is now a strong submission for the exercise.

---

## Suggested Improvement Targets
1. Keep move operations as minimal as practical.
2. Prefer the shortest clear form for the `is_valid()` and `get()` implementations.

---

## Final Verdict
- **Task compliance:** High
- **Correctness on provided tests:** Pass
- **Correctness against the written specification:** Pass
- **Teaching value:** Strong RAII example with good ownership semantics

## Self-Check Questions

### 1. What does the acronym RAII stand for, and what two events does it bind to the object's constructor and destructor?
**Your answer:**
Resource acquisition is initialization. It states that object constructor acquires the raw resource and destructor must release it.

**Evaluation:**
Correct. You captured both parts: what RAII means and how acquisition/release maps to constructor/destructor lifetime.

**Suggested polished answer:**
RAII stands for Resource Acquisition Is Initialization. The constructor acquires the resource, and the destructor releases it.

### 2. Why does RAII automatically provide a degree of exception safety?
**Your answer:**
RAII guarantees that resource is freed when object is destructed - either when going out of scope or when stack is unwinding.

**Evaluation:**
Correct. This is the core exception-safety benefit: deterministic cleanup during normal flow and unwinding.

**Suggested polished answer:**
RAII provides exception safety because destructors run automatically when scope ends, including during stack unwinding, so owned resources are still released.

### 3. List the five special member functions covered by the Rule of 5.
**Your answer:**
Desctructor, copy constructor, copy assignment, move constructor, move assignment.

**Evaluation:**
Conceptually correct. Minor spelling issue only.

**Suggested polished answer:**
Destructor, copy constructor, copy assignment operator, move constructor, and move assignment operator.

### 4. Under what circumstances should you follow the Rule of 0 instead ofcthe Rule of 5?
**Your answer:**
Under most circumstances rule of 0 should be followed, only when we acquire a raw resource, then we should follow rule of 3/5.

**Evaluation:**
Correct idea. This answer is strong conceptually: prefer Rule of 0 unless your type directly manages ownership of a resource.

**Suggested polished answer:**
Prefer the Rule of 0 when your class does not directly own raw resources and can rely on existing RAII types. Use Rule of 3/5 when the class itself manages ownership (for example, a raw handle, pointer, socket, or file descriptor).

### 5. Why is the copy constructor deleted in the `FileHandle` class rather than implemented?
**Your answer:**
Only one FileHandle should control this resource. If we allowed copy multiple instances could try to access the same resource. With copy constructor deleted there is at most one owner of the resource.

**Evaluation:**
Correct. This explains unique ownership and why double-management would be dangerous.

**Suggested polished answer:**
Copying is deleted to enforce unique ownership. If two objects owned the same descriptor, both might attempt to close it, causing double-close bugs and invalid resource state.

### 6. What happens if a destructor throws an exception during stack unwinding?
**Your answer:**
If exception is caught and handled nothing happens, when it's propagated program calls std::terminate() as desctructor is implicitly noexcept.

**Evaluation:**
Mostly correct. The key rule is right: if a destructor throws during stack unwinding and the exception escapes, the program terminates.

**Suggested polished answer:**
If a destructor throws during stack unwinding and that exception escapes the destructor, the program calls `std::terminate()`. That is why destructors should be non-throwing.

### 7. Why should move constructors and move assignment operators be marked `noexcept`?
**Your answer:**
It should perform only basic operations which shouldn't throw. Some objects may also prefer copying over moving when move isn't marked as noexcept.

**Evaluation:**
Correct. Good mention of library behavior: containers like vector may use copy instead of move if move is not `noexcept`.

**Suggested polished answer:**
Marking move operations `noexcept` communicates that transfer of ownership is safe and non-throwing, and it allows standard containers to prefer move operations during reallocation for better performance and stronger guarantees.

### 8. What value does `FileHandle` use as its invalid sentinel, and why is that value conventional for POSIX file descriptors?
**Your answer:**
In POSIX file descriptors always have non-negative integers as id, using negative id indicates invalid value.

**Evaluation:**
Partially correct. You explained the convention, but you should explicitly name the sentinel value used by this class.

**Suggested polished answer:**
`FileHandle` uses `-1` as the invalid sentinel. This is conventional because valid POSIX file descriptors are non-negative integers, so `-1` naturally represents "no valid descriptor".

### 9. Describe the steps that `reset(int new_handle)` must perform to avoid leaking the previously held resource.
**Your answer:**
It has to check if current handle has valid file descriptor, if yes than it realeases the resources. After that a new resource is obtained.

**Evaluation:**
Mostly correct. Important detail to include: even when there is no old valid descriptor, the new handle still must be stored.

**Suggested polished answer:**
`reset(new_handle)` should:
1. Return early if `new_handle` is the same as the current handle.
2. If the current handle is valid, close/release it.
3. Store `new_handle` as the new owned descriptor.

### 10. If you removed the self-assignment check from `operator=(FileHandle&&)`, what bug could occur?
**Your answer:**
Without that resource could be destroyed before being moved.

**Evaluation:**
Correct direction. In self-move assignment, you can accidentally close your own descriptor and then assign an invalid or already-released value back to yourself.

**Suggested polished answer:**
Without the self-assignment check, `obj = std::move(obj)` could close the resource owned by `obj` and then move from the same object, leaving it in an incorrect state or causing accidental resource loss.

---

## Summary
- Overall understanding: good
- Strong areas: RAII lifetime model, Rule of 5 members, unique ownership, `noexcept` motivation
- Needs tightening: precision in wording and explicitness of edge-case details (for example naming `-1` directly, reset sequence details)
- Estimated score: 8/10
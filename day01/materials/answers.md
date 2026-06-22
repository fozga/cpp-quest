## Self-Check Questions

### 1. What does the acronym RAII stand for, and what two events does it bind to the object's constructor and destructor?
Resource acquisition is initialization. It states that object constructor acquires the raw resource and destructor must release it.
### 2. Why does RAII automatically provide a degree of exception safety?
RAII guarantees that resource is freed when object is destructed - either when going out of scope or when stack is unwinding.
### 3. List the five special member functions covered by the Rule of 5.
Desctructor, copy constructor, copy assignment, move constructor, move assignment.
### 4. Under what circumstances should you follow the Rule of 0 instead ofcthe Rule of 5?
Under most circumstances rule of 0 should be followed, only when we acquire a raw resource, then we should follow rule of 3/5.
### 5. Why is the copy constructor deleted in the `FileHandle` class rather than implemented?
Only one FileHandle should control this resource. If we allowed copy multiple instances could try to access the same resource. With copy constructor deleted there is at most one owner of the resource.
### 6. What happens if a destructor throws an exception during stack unwinding?
If exception is caught and handled nothing happens, when it's propagated program calls std::terminate() as desctructor is implicitly noexcept.
### 7. Why should move constructors and move assignment operators be marked `noexcept`?
It should perform only basic operations which shouldn't throw. Some objects may also prefer copying over moving when move isn't marked as noexcept.
### 8. What value does `FileHandle` use as its invalid sentinel, and why is that value conventional for POSIX file descriptors?
In POSIX file descriptors always have non-negative integers as id, using negative id indicates invalid value.
### 9. Describe the steps that `reset(int new_handle)` must perform to avoid leaking the previously held resource.
It has to check if current handle has valid file descriptor, if yes than it realeases the resources. After that a new resource is obtained.
### 10. If you removed the self-assignment check from `operator=(FileHandle&&)`, what bug could occur?
Without that resource could be destroyed before being moved.
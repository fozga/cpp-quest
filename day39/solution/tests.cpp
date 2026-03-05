#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    TaskList tl;
    assert(tl.size() == 0);

    int id1 = tl.add_task("Design API").id;
    int id2 = tl.add_task("Write skeleton", TaskStatus::InProgress).id;
    assert(id1 == 1);
    assert(id2 == 2);
    assert(tl.size() == 2);

    assert(tl.list_all().size() == 2);
    assert(tl.list_by_status(TaskStatus::Todo).size() == 1);
    assert(tl.list_by_status(TaskStatus::InProgress).size() == 1);

    std::cout << "[PASS] Day39 design skeleton compiles and runs" << std::endl;
    std::cout << "All day-39 tests passed." << std::endl;
    return 0;
}

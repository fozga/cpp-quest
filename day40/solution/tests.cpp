#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    TaskList tl;

    // Add tasks — capture IDs immediately since references may be
    // invalidated by subsequent push_back operations.
    int id1 = tl.add_task("Buy groceries").id;
    int id2 = tl.add_task("Write report", TaskStatus::InProgress).id;
    int id3 = tl.add_task("Clean house", TaskStatus::Done, "2025-12-31").id;
    int id4 = tl.add_task("Plan vacation", TaskStatus::Todo, "2026-01-15").id;
    assert(tl.size() == 4);
    assert(id1 == 1 && id2 == 2 && id3 == 3 && id4 == 4);
    std::cout << "[PASS] add_task assigns unique increasing IDs" << std::endl;

    // list_all
    auto all = tl.list_all();
    assert(all.size() == 4);
    std::cout << "[PASS] list_all returns all tasks" << std::endl;

    // list_by_status
    auto todos = tl.list_by_status(TaskStatus::Todo);
    assert(todos.size() == 2);
    auto in_progress = tl.list_by_status(TaskStatus::InProgress);
    assert(in_progress.size() == 1);
    assert(in_progress[0].title == "Write report");
    auto done = tl.list_by_status(TaskStatus::Done);
    assert(done.size() == 1);
    assert(done[0].due_date.has_value());
    assert(done[0].due_date.value() == "2025-12-31");
    std::cout << "[PASS] list_by_status filters correctly" << std::endl;

    // find_task
    auto* found = tl.find_task(id2);
    assert(found != nullptr);
    assert(found->title == "Write report");
    auto* not_found = tl.find_task(999);
    assert(not_found == nullptr);
    std::cout << "[PASS] find_task" << std::endl;

    // remove_task
    assert(tl.remove_task(id2) == true);
    assert(tl.size() == 3);
    assert(tl.find_task(id2) == nullptr);
    assert(tl.remove_task(id2) == false);  // already removed
    assert(tl.remove_task(999) == false);  // never existed
    std::cout << "[PASS] remove_task" << std::endl;

    // Verify remaining tasks
    auto remaining = tl.list_all();
    assert(remaining.size() == 3);
    for (const auto& t : remaining)
        assert(t.id != id2);
    std::cout << "[PASS] remaining tasks consistent" << std::endl;

    // Empty list edge case
    TaskList empty;
    assert(empty.size() == 0);
    assert(empty.list_all().empty());
    assert(empty.list_by_status(TaskStatus::Todo).empty());
    assert(empty.find_task(1) == nullptr);
    assert(empty.remove_task(1) == false);
    std::cout << "[PASS] empty TaskList edge cases" << std::endl;

    std::cout << "Day40 final project tests passed." << std::endl;
    return 0;
}

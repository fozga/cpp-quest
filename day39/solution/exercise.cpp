#include "exercise.hpp"
#include <algorithm>

Task& TaskList::add_task(std::string title, TaskStatus status,
                         std::optional<std::string> due) {
    tasks_.push_back(Task{next_id_++, std::move(title), status, std::move(due)});
    return tasks_.back();
}

bool TaskList::remove_task(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& t) { return t.id == id; });
    if (it == tasks_.end()) return false;
    tasks_.erase(it);
    return true;
}

Task* TaskList::find_task(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& t) { return t.id == id; });
    return it != tasks_.end() ? &(*it) : nullptr;
}

const Task* TaskList::find_task(int id) const {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& t) { return t.id == id; });
    return it != tasks_.end() ? &(*it) : nullptr;
}

std::vector<Task> TaskList::list_all() const { return tasks_; }

std::vector<Task> TaskList::list_by_status(TaskStatus status) const {
    std::vector<Task> result;
    for (const auto& t : tasks_)
        if (t.status == status) result.push_back(t);
    return result;
}

std::size_t TaskList::size() const noexcept { return tasks_.size(); }

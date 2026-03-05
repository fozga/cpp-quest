#pragma once
#include <string>
#include <vector>
#include <optional>

enum class TaskStatus { Todo, InProgress, Done };

struct Task {
    int id = 0;
    std::string title;
    TaskStatus status = TaskStatus::Todo;
    std::optional<std::string> due_date;
};

class TaskList {
public:
    TaskList() = default;

    Task& add_task(std::string title,
                   TaskStatus status = TaskStatus::Todo,
                   std::optional<std::string> due = std::nullopt);
    bool remove_task(int id);
    Task* find_task(int id);
    const Task* find_task(int id) const;
    std::vector<Task> list_all() const;
    std::vector<Task> list_by_status(TaskStatus status) const;
    std::size_t size() const noexcept;

private:
    std::vector<Task> tasks_;
    int next_id_ = 1;
};

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;

    explicit Node(std::string node_name);
    ~Node();

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
};

using NodePtr = std::shared_ptr<Node>;
using WeakNodePtr = std::weak_ptr<Node>;

std::vector<NodePtr> make_cyclic_list(std::size_t n);
int get_live_node_count();
void reset_live_node_count();
void clear_list(std::vector<NodePtr>& list);

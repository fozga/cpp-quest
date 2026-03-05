#include "exercise.hpp"

#include <utility>

static int live_node_count = 0;

Node::Node(std::string node_name)
    : name(std::move(node_name))
{
    ++live_node_count;
}

Node::~Node()
{
    --live_node_count;
}

std::vector<NodePtr> make_cyclic_list(std::size_t n)
{
    if (n == 0) return {};

    std::vector<NodePtr> nodes;
    nodes.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        nodes.push_back(std::make_shared<Node>("node_" + std::to_string(i)));
    }

    for (std::size_t i = 0; i < n - 1; ++i) {
        nodes[i]->next = nodes[i + 1];
    }
    nodes[n - 1]->next = nodes[0];

    for (std::size_t i = 1; i < n; ++i) {
        nodes[i]->prev = nodes[i - 1];
    }
    nodes[0]->prev = nodes[n - 1];

    return nodes;
}

int get_live_node_count()
{
    return live_node_count;
}

void reset_live_node_count()
{
    live_node_count = 0;
}

void clear_list(std::vector<NodePtr>& list)
{
    if (list.empty()) return;

    NodePtr& first = list[0];
    for (auto& node : list) {
        if (node->next == first && node != first) {
            node->next.reset();
            break;
        }
    }

    if (!list.empty() && list[0]->next == list[0]) {
        list[0]->next.reset();
    }

    list.clear();
}

#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Pool with 16 slots of 256 bytes each — plenty for shared_ptr control
    // block + Node combined.
    SimplePool pool(16, 256);

    auto nodes = make_nodes_with_pool(10, pool);

    // Correct number of nodes created.
    assert(nodes.size() == 10);

    // At least one pool allocation per node (allocate_shared fuses the
    // control block and object, so exactly one slot per node is typical).
    assert(pool.allocation_count() >= 10);

    // Each node holds its index as its value.
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        assert(nodes[i]->value == static_cast<int>(i));
    }

    std::cout << "All tests passed.\n";
    std::cout << "Pool allocation count: " << pool.allocation_count() << "\n";
    return 0;
}

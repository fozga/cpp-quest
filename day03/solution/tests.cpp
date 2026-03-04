#include "exercise.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>

int main()
{
    reset_live_node_count();
    assert(get_live_node_count() == 0);

    constexpr std::size_t N = 4;

    {
        std::vector<NodePtr> list = make_cyclic_list(N);
        assert(get_live_node_count() == static_cast<int>(N));

        {
            NodePtr cur = list[0];
            for (std::size_t i = 0; i < N; ++i) {
                assert(cur != nullptr);
                assert(cur == list[i]);
                cur = cur->next;
            }
            assert(cur == list[0]);
        }

        for (std::size_t i = 1; i < N; ++i) {
            NodePtr prev_locked = list[i]->prev.lock();
            assert(prev_locked != nullptr);
            assert(prev_locked == list[i - 1]);
        }
        {
            NodePtr prev_of_first = list[0]->prev.lock();
            assert(prev_of_first != nullptr);
            assert(prev_of_first == list[N - 1]);
        }

        clear_list(list);
        assert(list.empty());
        assert(get_live_node_count() == 0);
    }

    std::cout << "day03: all tests passed\n";
    return 0;
}

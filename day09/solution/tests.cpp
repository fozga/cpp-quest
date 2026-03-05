#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    Widget* w = new Widget();
    Button* b = new Button();
    ToggleButton* tb = new ToggleButton(false);

    std::vector<Widget*> widgets = {w, b, tb};

    std::cout << "--- draw_all ---\n";
    draw_all(widgets);

    std::string names = collect_names(widgets);
    std::cout << "collected names: " << names << "\n";
    assert(names.find("Widget") != std::string::npos);
    assert(names.find("Button") != std::string::npos);
    assert(names.find("ToggleButton") != std::string::npos);

    assert(!tb->is_on());
    tb->toggle();
    assert(tb->is_on());
    tb->toggle();
    assert(!tb->is_on());

    for (auto* ptr : widgets) {
        delete ptr;
    }

    std::cout << "\n[day09] All tests passed.\n";
    return 0;
}

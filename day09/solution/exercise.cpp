#include "exercise.hpp"

#include <iostream>
#include <sstream>

std::string Widget::name() const {
    return "Widget";
}

void Widget::draw() const {
    std::cout << "[Widget] drawing base widget\n";
}

std::string Button::name() const {
    return "Button";
}

void Button::draw() const {
    std::cout << "[Button] drawing button\n";
}

ToggleButton::ToggleButton(bool initial_state) : on_(initial_state) {}

std::string ToggleButton::name() const {
    return "ToggleButton";
}

void ToggleButton::draw() const {
    if (on_) {
        std::cout << "[ToggleButton] ON\n";
    } else {
        std::cout << "[ToggleButton] OFF\n";
    }
}

void ToggleButton::toggle() {
    on_ = !on_;
}

bool ToggleButton::is_on() const noexcept {
    return on_;
}

void draw_all(const std::vector<Widget*>& widgets) {
    for (const auto* w : widgets) {
        w->draw();
    }
}

std::string collect_names(const std::vector<Widget*>& widgets) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < widgets.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << widgets[i]->name();
    }
    return oss.str();
}

#pragma once

#include <string>
#include <vector>

class Widget {
public:
    virtual ~Widget() = default;

    virtual std::string name() const;
    virtual void draw() const;
};

class Button : public Widget {
public:
    std::string name() const override;   // override: must match Widget::name()
    void draw() const override;          // override: must match Widget::draw()
};

class ToggleButton final : public Button {   // final: no further derivation
public:
    explicit ToggleButton(bool initial_state = false);

    std::string name() const override;       // override: must match Button/Widget::name()
    void draw() const override final;        // final: no subclass may override draw()

    void toggle();
    bool is_on() const noexcept;

private:
    bool on_ = false;
};

void draw_all(const std::vector<Widget*>& widgets);
std::string collect_names(const std::vector<Widget*>& widgets);

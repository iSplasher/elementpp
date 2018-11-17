#include "window.h"
#include <FL/Fl_Window.H>

USING_LIB_NAMESPACE
USING_PRIVATE_LIB_NAMESPACE

ui::Window::Window(const Props &props) : Component(props) {
    auto[width, height] = props->get<std::pair<double, double>>("size").value_or(std::make_pair<>(300, 300));
    window = std::make_shared<Fl_Window>(width, height);
    if (props->get<bool>("show").value_or(false)) {
        window->show();
    }
}

ui::Window::Window(Window&& other) noexcept : Component(std::move(other)),
window(std::move(other.window)) {}

ui::Window& ui::Window::operator=(Window&& other) noexcept {
    if (this == &other)
        return *this;
    Component::operator =(std::move(other));
    window = std::move(other.window);
    return *this;
}

ComponentInstPtr ui::Window::render() const {

    return Component::render();

}

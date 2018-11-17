#pragma once

#include "component.h"


class Fl_Window;

LIB_NAMESPACE_BEGIN

namespace ui
{

    struct Window : Component {

        Window() = default;

        Window(const Props &props);

        Window(Window&& other) noexcept;

        Window& operator=(Window&& other) noexcept;

        virtual ~Window() = default;

        ComponentInstPtr render() const override;

    private:

        std::shared_ptr<Fl_Window> window;

    };


}

LIB_NAMESPACE_END


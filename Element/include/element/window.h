#pragma once

#include "widget.h"
#include "core/painter.h"

#include <chrono>

using _privRWindow = struct GLFWwindow;

NAMESPACE_BEGIN

class ELEMENT_API Window : public Widget {
public:
	explicit Window(Window* parent = nullptr) : Window(Size(500, 300), parent) {}
	explicit Window(Size size, Window* parent = nullptr);
	virtual ~Window();

protected:
	// methods

	void update() override;

private:
	//methods
	/**
	 * \brief Painting can only be done on active window
	 */
	void setActive() const;

	static void mouseMovedCb(_privRWindow* r_window, double xpos, double ypos);
	static void mouseMovedHelper(Widget* w, Point p, MouseButton buttons);
	static void mousePressCb(_privRWindow* r_window, int button, int action, int mods);
	static void mousePressedHelper( Widget* w, bool btn_press, MouseButton buttons, KeyModifier modifiers, Point p, bool click, bool d_click, bool d_press );

	//abitrary data members
	MouseButton last_pressed_buttons = MouseButton::None; // buttons that were last pressed
	std::chrono::time_point<std::chrono::steady_clock> last_pressed; // time point a button was last pressed
	std::chrono::time_point<std::chrono::steady_clock> last_released; // time point a button was last released
	Rect button_press_rect; // a rect surrounding the point a button pressed
	int d_clicks_count = 0; // counts clicks until 2 is reached for doubleclick

	bool is_dragging = false;

	bool _inited = false;

	UniquePainter painter;
	// render window
	_privRWindow *r_window;

	friend class Painter;
	friend class Widget;
	friend class Application;
};

NAMESPACE_END
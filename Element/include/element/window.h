#pragma once

#include "widget.h"
#include "core/painter.h"

#include <chrono>

using _privRWindow = struct SDL_Window;
using _privCursor = struct GLFWcursor;

NAMESPACE_BEGIN

PRIV_NAMESPACE_BEGIN
static Direction windowHitTestHelper(Window* window, Point point);
NAMESPACE_END

class ELEMENT_API Window : public Widget {
public:
	explicit Window(Window* parent = nullptr) : Window(Rect(0, 0, 500, 300), parent) {}
	explicit Window(Rect rect, Window* parent = nullptr);
	virtual ~Window();

	void update() override;

	Property<float, Window> pixelRatio;

private:
	//methods
	/**
	 * \brief Painting can only be done on active window
	 */
	void setActive() const;
	void updateGeometry();

	static void applyWidgetCursor(Widget* w);
	static void applyWidgetResizeCursor(Widget* w, Direction dir);
	static bool resizeHelper(Widget* w, Point p, MouseButton buttons );
	static Direction inResizeRangeHelper(Widget* w, Point p);
	static void windowResizedCb(_privRWindow* r_window, int width, int height);
	static void windowMovedCb(_privRWindow* r_window, int xpos, int ypos);
	static void mouseMovedCb(_privRWindow* r_window, double xpos, double ypos);
	static void mouseMovedHelper(Widget* w, Point p, MouseButton buttons);
	static void mousePressCb(_privRWindow* r_window, int button, int action, int mods);
	static void mousePressedHelper( Widget* w, bool btn_press, MouseButton buttons, KeyModifier modifiers, Point p, bool click, bool d_click, bool d_press );

	//abitrary data members
	std::unique_ptr<PRIV_NAMESPACE::_Cursor> hresize_cursor = nullptr;
	std::unique_ptr<PRIV_NAMESPACE::_Cursor> vresize_cursor = nullptr;

	MouseButton last_pressed_buttons = MouseButton::None; // buttons that were last pressed
	std::chrono::time_point<std::chrono::steady_clock> last_pressed; // time point a button was last pressed
	std::chrono::time_point<std::chrono::steady_clock> last_released; // time point a button was last release
	Rect button_press_rect; // a rect surrounding the point a button pressed
	int d_clicks_count = 0; // counts clicks until 2 is reached for doubleclick

	bool is_dragging = false;
	bool _inited = false;
	Point current_pos;
	Size current_size;
	Widget* grabbed_widget = nullptr;

	UniquePainter painter;
	_privRWindow *r_window; // render window
	void* context = nullptr;

	friend class Painter;
	friend class Widget;
	friend class Application;
	friend static Direction PRIV_NAMESPACE::windowHitTestHelper(Window* window, Point point);
};

PRIV_NAMESPACE_BEGIN

static const std::string _winpointer = "window";

class _Cursor {
	void _newCursor(Cursor c);
	void _destroy();
	Cursor old_cursor = Cursor::None;
	_privCursor* cursor = nullptr;
	_privRWindow* r_window = nullptr;
public:
	explicit _Cursor(Cursor, _privRWindow*);
	~_Cursor();
	void apply(Cursor);
	void apply();
};
NAMESPACE_END

NAMESPACE_END
#pragma once

#include "widget.h"
#include "core/painter.h"

using _privRWindow = struct GLFWwindow;

NAMESPACE_BEGIN

class ELEMENT_API Window : public Widget {
public:
	explicit Window(Window* parent = nullptr) : Window(SizeF(500, 300), parent) {}
	explicit Window(SizeF size, Window* parent = nullptr);
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
	static void mousePressCb(_privRWindow* r_window, int button, int action, int mods);

	//abitrary data members
	double _old_mouse_x = 0;
	double _old_mouse_y = 0;
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
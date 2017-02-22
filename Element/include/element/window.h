#pragma once

#include "widget.h"
#include "core/painter.h"

typedef struct GLFWwindow _privRWindow;

NAMESPACE_BEGIN

class ELEMENT_API Window : public Widget {
public:
	explicit Window(Window* parent = nullptr) : Window(SizeF(500, 300), parent) {}
	explicit Window(SizeF size, Window* parent = nullptr);
	virtual ~Window();

protected:
	// methods

private:
	//methods
	/// <summary>
	/// Painting can only be done on active window.
	/// </summary>
	void setActive() const;

	virtual void update();
	void paint(Painter &painter) override;

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
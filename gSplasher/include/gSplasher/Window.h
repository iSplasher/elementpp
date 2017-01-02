#pragma once

#include "gSplasher/Widgets/TopBar.h"
#include "Utils/Painter.h"

typedef struct GLFWwindow _privRWindow;

NAMESPACE_BEGIN

/// <summary>
/// A window. 
/// </summary>
class GSPLASHER_API Window : public WidgetCore {
public:
	using TopBarPtr = std::unique_ptr<TopBar>;
	explicit Window(Window* parent = nullptr) : Window(Size(500, 300), parent) {}
	explicit Window(Size size, Window* parent = nullptr);
	virtual ~Window();

	// methods
	TopBarPtr& topBar() { return top_bar; }
	//Point pos() override;
	void move(Point new_p) override;

	Rect contentGeometry() override;

protected:
	// methods
	virtual void resizeEvent(ResizeEventPtr ev);


private:
	//methods
	/// <summary>
	/// Painting can only be done on active window.
	/// </summary>
	void setActive() const;

	void update();
	void paint(Painter &painter) override;

	//abitrary data members
	double _old_mouse_x = 0;
	double _old_mouse_y = 0;
	bool is_dragging = false;

	bool _inited = false;

	UniquePainter painter;
	// Top bar of Window
	TopBarPtr top_bar;
	// render window
	_privRWindow *r_window;

	friend class Painter;
	friend class Application;
	friend class WidgetCore;
	friend class TopBar;
};

NAMESPACE_END
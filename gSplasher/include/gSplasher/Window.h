#pragma once

#include "gSplasher/Widgets/TopBar.h"
#include "Utils/Painter.h"

typedef struct GLFWwindow _RWindow;

NAMESPACE_BEGIN

/// <summary>
/// A window. 
/// </summary>
class GSPLASHER_API gWindow : public gCoreWidget {
public:
	explicit gWindow(gWindow* parent = nullptr);
	virtual ~gWindow();

	// methods
	//Point pos() override;
	void move(gPoint new_p) override;
	void resize(gSize new_s) override;

protected:
	// methods

	_RWindow *r_window;

private:
	//methods

	/// <summary>
	/// Painting can only be done on active window.
	/// </summary>
	void setActive() const;

	void update();
	void paint(gPainter &painter) override;

	//abitrary data members
	double _old_mouse_x = 0;
	double _old_mouse_y = 0;
	bool is_dragging = false;

	bool _inited = false;

	// Top bar of Window
	std::unique_ptr<gTopBar> top_bar;
	UniquePainter painter;

	friend class gPainter;
	friend class gApplication;
	friend class gCoreWidget;
	friend class gTopBar;
};

NAMESPACE_END
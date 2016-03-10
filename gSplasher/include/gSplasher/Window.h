#pragma once

#include "gSplasher/Widgets/TopBar.h"
#include "Utils/Painter.h"

typedef struct GLFWwindow _privRWindow;

NAMESPACE_BEGIN

/// <summary>
/// A window. 
/// </summary>
class GSPLASHER_API gWindow : public gCoreWidget {
public:
	explicit gWindow(gWindow* parent = nullptr) : gWindow(gSize(500, 300), parent) {}
	explicit gWindow(gSize size, gWindow* parent = nullptr);
	virtual ~gWindow();

	// methods
	//Point pos() override;
	void move(gPoint new_p) override;
	void resize(gSize new_s) override;

protected:
	// methods


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

	UniquePainter painter;
	// Top bar of Window
	std::unique_ptr<gTopBar> top_bar;
	// render window
	_privRWindow *r_window;

	friend class gPainter;
	friend class gApplication;
	friend class gCoreWidget;
	friend class gTopBar;
	friend class priv::LayoutImpl;
};

NAMESPACE_END
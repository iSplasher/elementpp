#pragma once

#include "gSplasher/Widgets/TopBar.h"
#include "Utils/Painter.h"

typedef struct GLFWwindow _privRWindow;

NAMESPACE_BEGIN

/// <summary>
/// A window. 
/// </summary>
class GSPLASHER_API gWindow : public CoreWidget {
public:
	using TopBar = std::unique_ptr<gTopBar>;
	explicit gWindow(gWindow* parent = nullptr) : gWindow(gSize(500, 300), parent) {}
	explicit gWindow(gSize size, gWindow* parent = nullptr);
	virtual ~gWindow();

	// methods
	TopBar& topBar() { return top_bar; }
	//Point pos() override;
	void move(gPoint new_p) override;

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
	TopBar top_bar;
	// render window
	_privRWindow *r_window;

	friend class Painter;
	friend class gApplication;
	friend class CoreWidget;
	friend class gTopBar;
	friend class priv::LayoutImpl;
};

NAMESPACE_END
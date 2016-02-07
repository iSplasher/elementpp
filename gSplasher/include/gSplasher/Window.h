#pragma once

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
	void move(gPoint new_p);
	//void resize(Size new_s) override;

protected:
	// methods

	_RWindow *r_window;

private:
	//methods

	/// <summary>
	/// Painting can only be done on active window.
	/// </summary>
	void setActive() const;

	void update() override;
	void paint(gPainter &painter);

	//abitrary data members
	int _old_mouse_x = 0;
	int _old_mouse_y = 0;

	friend class gPainter;
	friend class gApplication;
};

NAMESPACE_END
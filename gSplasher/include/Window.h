#pragma once

#include "../include/Widget.h"

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
	void update() override;
	//Point pos() override;
	//void move(Point new_p) override;
	//void resize(Size new_s) override;
	/// <summary>
	/// Activates the window. Note: painting can only be done on active window.
	/// </summary>
	void setActive() const;

protected:
	// methods

	_RWindow *r_window;

private:
	//methods
	void generateMouseMove();

	//abitrary data members
	int _old_mouse_x = 0;
	int _old_mouse_y = 0;


};

NAMESPACE_END
#pragma once

#include "Global.h"
#include "Layout.h"
#include "Core.h"
#include "Event.h"
#include "Utils/CoreUtils.h"

#include <SFML/Graphics.hpp>


NAMESPACE_BEGIN

/// <summary>
/// Core widget. A very basic widget. Can be used as a container for other widgets
/// </summary>
class GSPLASHER_API gCoreWidget : public gCore { //: public gLayoutable {
public:

	enum MoveState {
		Normal = 0,
		Moving = 1
	};

	enum State {
		Acivated = 0,
		Disabled = 1,
	};

	// *structers
	explicit gCoreWidget(gCoreWidget *parent = nullptr);
	//gCoreWidget(const gCoreWidget&);
	//explicit gCoreWidget(const gWidgetProperties s, gCoreWidget *parent = nullptr);
	virtual ~gCoreWidget() = default;

	// member methods
	//virtual void paint(Painter &painter, gWidgetProperties &widget_style) {}
	virtual void update();// override;
	virtual void event(EventPtr ev);
	virtual Point pos();
	virtual void move(Point new_p);
	virtual void resize(Size new_s);
	virtual void resize(int width, int height) { resize(Size(width, height)); }
	Point mapToGlobal(Point p);
	//void setLayout(gLayout&);
	bool underMouse() const { return under_mouse; }
	unsigned width() const { return style.size.width; }
	unsigned height() const { return style.size.height; }


	// data members
	gWidgetProperties style;

protected:
	// member methods
	virtual void mousePressEvent(MouseEventPtr ev);
	virtual void mouseMoveEvent(MouseEventPtr ev);
	virtual void mouseReleaseEvent(MouseEventPtr ev);
	virtual void redraw() {}

	// data members
	gCoreWidget *parent_widget;
	MoveState move_state = Normal;
	//gLayout &m_layout;

private:
	Point move_offset;
	bool under_mouse;

friend class gApplication;
};

/// <summary>
/// A window. 
/// </summary>
class GSPLASHER_API gWindow : public gCoreWidget {
public:
	explicit gWindow(gWindow* parent = nullptr);
	virtual ~gWindow();

	// methods
	void update() override;
	Point pos() override;
	void move(Point new_p) override;
	void resize(Size new_s) override;

protected:
	// methods

	std::unique_ptr<sf::RenderWindow> r_window;

private:
	//methods
	void generateMouseMove();

	//abitrary data members
	int _old_mouse_x = 0;
	int _old_mouse_y = 0;


};

NAMESPACE_END
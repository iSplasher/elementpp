#pragma once

#include "Global.h"
#include "Layout.h"
#include "Core.h"
#include "Event.h"
#include "Utils/Primitives.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN

class gPainter;
class gWindow;

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
	virtual ~gCoreWidget();

	// member methods
	virtual void paint(gPainter &painter);
	virtual void update();// override;
	virtual void event(EventPtr ev);
	gPoint pos() const { return p; };
	virtual void move(gPoint new_p) { p = new_p; }
	void move(int x, int y) { move(gPoint(x, y)); }
	//virtual void resize(Size new_s);
	//virtual void resize(int width, int height) { resize(Size(width, height)); }
	//Point mapToGlobal(Point p);
	//void setLayout(gLayout&);
	bool underMouse() const { return under_mouse; }
	virtual gSize size() const { return gSize(500, 300); }
	
	void setFont(gFont font) { _font = font; }
	gFont& font() { return _font; }

protected:
	// member methods
	virtual void mousePressEvent(MouseEventPtr ev);
	virtual void mouseMoveEvent(MouseEventPtr ev);
	virtual void mouseReleaseEvent(MouseEventPtr ev);

	// data members
	gWindow *parent_window = nullptr;
	gCoreWidget *parent_widget;
	MoveState move_state = Normal;
	//gLayout &m_layout;
	gPoint p;
	gFont _font;

private:
	//Point move_offset;
	bool under_mouse;
	PainterContext *this_paint = nullptr;

friend class gPainter;
};

NAMESPACE_END
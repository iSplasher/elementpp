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
	void update();// override;
	virtual void event(EventPtr ev);
	gPoint pos() const { return p; };
	virtual void move(gPoint new_p) { p = new_p; }
	void move(int x, int y) { move(gPoint(x, y)); }
	virtual void resize(gSize new_s);
	virtual void resize(int width, int height) { resize(gSize(width, height)); }
	//Point mapToGlobal(Point p);
	//void setLayout(gLayout&);
	bool underMouse() const { return under_mouse; }
	virtual gSize size() const { return _size; }
	
	void setFont(gFont font) { _font = font; }
	gFont& font() { return _font; }

	/// <summary>
	/// Translates the coordinates p of this widget to a coordinate p in the parent widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in the parent widget</returns>
	gPoint mapToParent(gPoint p) const;

	/// <summary>
	/// Translates the coordinates p of the parent widget to a coordinate p in this widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in this widget</returns>
	gPoint mapFromParent(gPoint p) const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="p"></param>
	/// <returns></returns>
	gPoint mapFromGlobal(gPoint p);
	gPoint mapToGlobal(gPoint p);

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
	PainterContext *this_paint = nullptr;

private:
	void updateChildren();

	//Point move_offset;
	bool under_mouse = false;
	gSize _size = gSize(500, 300);

friend class gPainter;
};

using WidgetPtr = std::shared_ptr<gCoreWidget>;
using UniqueWidgetPtr = std::unique_ptr<gCoreWidget>;


NAMESPACE_END
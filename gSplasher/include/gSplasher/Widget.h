#pragma once

#include "Layout.h"
#include "Event.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN

class gPainter;
class gWindow;

/// <summary>
/// Core widget. A very basic widget. Can be used as a container for other widgets
/// </summary>
class GSPLASHER_API gCoreWidget : public gLayoutable {
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
	void update() override;
	virtual void event(EventPtr ev);
	void setLayout(gLayout& new_layout);
	bool underMouse() const { return under_mouse; }

	void setFont(gFont font) { _font = font; }
	gFont& font() { return _font; }

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
	gFont _font;
	PainterContext *this_paint = nullptr;

private:
	void updateChildren();

	//Point move_offset;
	bool under_mouse = false;

friend class gPainter;
};

using WidgetPtr = std::shared_ptr<gCoreWidget>;
using UniqueWidgetPtr = std::unique_ptr<gCoreWidget>;


NAMESPACE_END
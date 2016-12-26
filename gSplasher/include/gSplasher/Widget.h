#pragma once

#include "Layout.h"
#include "Event.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN

class Painter;
class Window;

/// <summary>
/// Core widget. A very basic widget. Can be used as a container for other widgets
/// </summary>
class GSPLASHER_API WidgetCore : public priv::LayoutCore {
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
	explicit WidgetCore(WidgetCore *parent = nullptr);
	//WidgetCore(const WidgetCore&);
	//explicit WidgetCore(const gWidgetProperties s, WidgetCore *parent = nullptr);
	virtual ~WidgetCore();

	// member methods
	Window* parentWindow() const { return parent_window; }
	virtual void paint(Painter &painter);
	void update() override;
	virtual void event(EventPtr ev);
	Point pos() const override;
	void setParent(WidgetCore* new_parent);
	void setLayout(Layout& new_layout);
	bool underMouse() const { return under_mouse; }

	void setFont(gFont font) { _font = font; }
	gFont& font() { return _font; }

	/// <summary>
	/// Translates the coordinates p of this widget to a coordinate p in the parent widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in the parent widget</returns>
	Point mapToParent(Point p) const;

	/// <summary>
	/// Translates the coordinates p of the parent widget to a coordinate p in this widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in this widget</returns>
	Point mapFromParent(Point p) const;

	/// <summary>
	/// Translates the coordinates p from the containing window's
	/// </summary>
	/// <remarks>
	/// If there is no containing window, no translation will be done
	/// </remarks>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordinate from containing window</returns>
	Point mapFromWindow(Point p) const;

	/// <summary>
	/// Translates the coordinates p to the containing window's
	/// </summary>
	/// <remarks>
	/// If there is no containing window, no translation will be done
	/// </remarks>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordinate in containing window</returns>
	Point mapToWindow(Point p) const;

	/// <summary>
	/// Translates the coordinates p from the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine from desktop</returns>
	Point mapFromGlobal(Point p) const;

	/// <summary>
	/// Translates the coordinates p to the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine in desktop</returns>
	Point mapToGlobal(Point p) const;


protected:

	struct Drag {
		bool is_draggable = true;
		Point start_mouse_pos;
		Point start_pos;
	};

	// member methods
	virtual void mousePressEvent(MouseEventPtr ev);
	virtual void mouseMoveEvent(MouseEventPtr ev);
	virtual void mouseReleaseEvent(MouseEventPtr ev);

	// data members
	Window *parent_window = nullptr;
	WidgetCore *parent_widget;
	MoveState move_state = Normal;
	gFont _font;
	PainterContext *this_paint = nullptr;
	Drag drag;

private:
	void updateChildren();

	//Point move_offset;
	bool under_mouse = false;

friend class Painter;
friend class Layout;
};

using WidgetPtr = std::shared_ptr<WidgetCore>;
using UniqueWidgetPtr = std::unique_ptr<WidgetCore>;


NAMESPACE_END
#pragma once

#include "layout.h"
#include "property.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN

class Painter;
class RWindow;

/// <summary>
/// Component widget. A very basic widget. Can be used as a container for other widgets
/// </summary>
class ELEMENT_API Widget : public PRIV_NAMESPACE::LayoutElement {
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
	explicit Widget(Widget *parent = nullptr);
	//Widget(const Widget&);
	//explicit Widget(const gWidgetProperties s, Widget *parent = nullptr);
	virtual ~Widget();

	// member methods
	virtual void paint(Painter &painter);
	void update() override;

	Property<bool, Widget> underMouse;
	//Property<gFont, Widget> font;

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
	Point mapFromWindow(Point p);

	/// <summary>
	/// Translates the coordinates p to the containing window's
	/// </summary>
	/// <remarks>
	/// If there is no containing window, no translation will be done
	/// </remarks>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordinate in containing window</returns>
	Point mapToWindow(Point p);

	/// <summary>
	/// Translates the coordinates p from the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine from desktop</returns>
	Point mapFromGlobal(Point p);

	/// <summary>
	/// Translates the coordinates p to the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine in desktop</returns>
	Point mapToGlobal(Point p);


protected:

	struct Drag {
		bool is_draggable = true;
		Point start_mouse_pos;
		Point start_pos;
	};

	// data members
	RWindow *parent_window = nullptr;
	Widget *parent_widget;
	MoveState move_state = Normal;
	PainterContext *this_paint = nullptr;
	Drag drag;

private:
	//Point move_offset;

friend class Painter;
friend class Layout;
};

using WidgetPtr = std::unique_ptr<Widget>;

NAMESPACE_END
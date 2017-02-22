#pragma once

#include "layout.h"
#include "property.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN

class Painter;
class Window;

/// <summary>
/// Component widget. A very basic widget. Can be used as a container for other widgets
/// </summary>
class ELEMENT_API Widget : public PRIV_NAMESPACE::Layoutable {
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
	Property<Font> font;
	Property<Color> borderColor;
	Property<Color> backgroundColor;
	Property<Color> foregroundColor;
	PropertyView< RectF > contentGeometry;
	PropertyView< SizeF > contentSize;

	Property<float> borderRadiusTopLeft;
	Property<float> borderRadiusTopRight;
	Property<float> borderRadiusBottomLeft;
	Property<float> borderRadiusBottomRight;

	bool paintWidget = true;

	/// <summary>
	/// Translates the coordinates p of this widget to a coordinate p in the parent widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in the parent widget</returns>
	PointF mapToParent(PointF p) const;
	RectF mapToParent(RectF p) const { return RectF(mapToParent(p.pos()), p.size()); }

	/// <summary>
	/// Translates the coordinates p of the parent widget to a coordinate p in this widget
	/// </summary>
	/// <param name="p">Coordinates to translate</param>
	/// <returns>The translated coordinate in this widget</returns>
	PointF mapFromParent(PointF p) const;
	RectF mapFromParent(RectF p) const { return RectF(mapFromParent(p.pos()), p.size()); }

	/// <summary>
	/// Translates the coordinates p from the containing window's
	/// </summary>
	/// <remarks>
	/// If there is no containing window, no translation will be done
	/// </remarks>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordinate from containing window</returns>
	PointF mapFromWindow(PointF p);
	RectF mapFromWindow(RectF p) { return RectF(mapFromWindow(p.pos()), p.size()); }

	/// <summary>
	/// Translates the coordinates p to the containing window's
	/// </summary>
	/// <remarks>
	/// If there is no containing window, no translation will be done
	/// </remarks>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordinate in containing window</returns>
	PointF mapToWindow(PointF p);
	RectF mapToWindow(RectF p) { return RectF(mapToWindow(p.pos()), p.size()); }

	/// <summary>
	/// Translates the coordinates p from the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine from desktop</returns>
	PointF mapFromGlobal(PointF p);
	RectF mapFromGlobal(RectF p) { return RectF(mapFromGlobal(p.pos()), p.size()); }

	/// <summary>
	/// Translates the coordinates p to the desktop coordinates
	/// </summary>
	/// <param name="p">Coordinate to translate</param>
	/// <returns>The translated coordine in desktop</returns>
	PointF mapToGlobal(PointF p);
	RectF mapToGlobal(RectF p) { return RectF(mapToGlobal(p.pos()), p.size()); }


protected:

	struct Drag {
		bool is_draggable = true;
		Point start_mouse_pos;
		Point start_pos;
	};

	// data members
	Window *parent_window = nullptr;
	Widget *parent_widget;
	MoveState move_state = Normal;
	PainterContext *this_paint = nullptr;
	Drag drag;

private:

	void setParent(Element*) override;

friend class Painter;
friend class Layout;
};

using WidgetPtr = std::unique_ptr<Widget>;

NAMESPACE_END
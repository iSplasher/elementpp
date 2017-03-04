#pragma once

#include "element/widget.h"

NAMESPACE_BEGIN

class Painter;

class ELEMENT_API Pen {
public:
	enum class Cap;
	enum class Join;

	Pen() = default;
	explicit Pen(Painter &painter) : Pen(painter, Color(0, 0, 0)) {}
	explicit Pen(Painter &painter, Color color);
	~Pen();

	/// <summary>
	/// Set a new color to this pen
	/// </summary>
	void setColor(Color color);

	Color color() const { return c_color; }

	/// <summary>
	/// Set a new line width to this pen
	/// </summary>
	void setWidth(float width);

	/// <summary>
	/// Get current width
	/// </summary>
	/// <returns>Current width</returns>
	float width() const { return c_width; }

	/// <summary>
	/// Set how the end of the line is drawn
	/// </summary>
	void setCap(Cap cap);

	/// <summary>
	/// Set how the corners are drawn
	/// </summary>
	void setJoin(Join join);

private:
	/// <summary>
	/// Applies the current pen style
	/// </summary>
	void apply() const;

	Painter *painter = nullptr;
	PainterContext* pc = nullptr;
	float c_width = 0.1f;
	Color c_color;

	friend class Painter;
};

class ELEMENT_API Brush {
public:
	Brush() = default;
	explicit Brush(Painter &painter) : Brush(painter, Color(0, 0, 0)){}
	explicit Brush(Painter &painter, Color color);

	~Brush();

	bool operator==(const Brush& rhs ) const {
		return c_color == rhs.c_color;
	}

	bool operator!=(const Brush& rhs ) const { return !( *this == rhs ); }

	/// <summary>
	/// Set a new color to this brush
	/// </summary>
	void setColor(Color color);

	Color color() const { return c_color; }

	//void setGradient();
	//void setPattern();

private:
	/// <summary>
	/// Applies the current brush style
	/// </summary>
	void apply() const;

	Painter* painter = nullptr;
	PainterContext* pc = nullptr;
	Color c_color;

	friend class Painter;
};

class TopBar;

class ELEMENT_API Painter {
public:
	Painter(Window*);
	~Painter();

	/// <summary>
	/// Begin painting
	/// </summary>
	const Painter& begin(float pixel_ratio=1);

	/// <summary>
	/// End painting
	/// </summary>
	const Painter& end();

	/// <summary>
	/// Set new pen
	/// </summary>
	/// <param name="pen">New pen</param>
	const Painter& setPen(Pen &pen);

	/// <summary>
	/// Retrieve current pen
	/// </summary>
	/// <returns>Current Pen</returns>
	Pen &pen() const { return *p; }

	/// <summary>
	/// Saves the current state
	/// </summary>
	/// <remarks>A matching restore() call must be used</remarks>
	const Painter& save();

	/// <summary>
	/// Restores saved state
	/// </summary>
	/// <remarks>A save() must have been called before calling this method</remarks>
	const Painter& restore();

	/// <summary>
	/// Resets to default state
	/// </summary>
	/// <remarks>This does not affect the state stack</remarks>
	const Painter& reset();

	const Painter& resetPen();
	const Painter& resetBrush();

	/// <summary>
	/// Sets the transparency applied to all rendered shapes.
	/// Already transparent paths will get proportionally more transparent as well.
	/// </summary>
	/// <param name="alpha">new alpha value between 0 and 1</param>
	const Painter& setGlobalAlpha(float alpha) const;

	/// <summary>
	/// Set new brush
	/// </summary>
	/// <param name="brush">New brush</param>
	const Painter& setBrush(Brush &brush);

	/// <summary>
	/// Retrieve current brush
	/// </summary>
	/// <returns>Current Brush</returns>
	Brush &brush() const { return *b; }

	
	/**
	 * \brief Clipping allows you to clip the painting into a rectangle.
	 * \param rect rectangle
	 * \return painter
	 */
	const Painter& clip(Rect rect) const;
	const Painter& clip(RectI rect) const { return clip(Rect(rect)); };


	/**
	 * \brief Reset previously set clipping.
	 * \return painter
	 */
	const Painter& resetClip() const;

	/// <summary>
	/// Draw a rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	const Painter& drawRect(Rect rect) const;
	const Painter& drawRect(RectI rect) const { return drawRect(Rect(rect)); }

	/// <summary>
	/// Draw a rounded rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	/// <param name="radius">Radius of the rect corners</param>
	const Painter& drawRoundedRect(Rect rect, float radius) const;
	const Painter& drawRoundedRect(RectI rect, int radius) const { return drawRoundedRect(Rect(rect), radius); }

	const Painter& drawRoundedRect(Rect rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left) const;
	const Painter& drawRoundedRect(RectI rect, int rad_top_left, int rad_top_right, int rad_bottom_right, int rad_bottom_left) const {
		return drawRoundedRect(Rect(rect), rad_top_left, rad_top_right, rad_bottom_right, rad_bottom_left);
	}

	/// <summary>
	/// Draw an ellipse shape
	/// </summary>
	/// <param name="center">Center of ellipse</param>
	/// <param name="size">Width and height of ellipse</param>
	const Painter& drawEllipse(Point center, Size size) const;

	/// <summary>
	/// Draw a circle shape
	/// </summary>
	/// <param name="center">Center of circle</param>
	/// <param name="radius">Circle radius</param>
	const Painter& drawCircle(Point center, float radius) const;

	/// <summary>
	/// Draw a line
	/// </summary>
	/// <param name="start">Start point of line</param>
	/// <param name="end">End point of line</param>
	const Painter& drawLine(Point start, Point end) const;

private:

	// helper methods
	void paintWidget(Widget* w);
	void translate(Rect &r) const;
	void translate(Point &p) const;
	void beginPath() const;
	void applyPB() const;

	// TODO: Create a save-stack struct to store old objects instead!
	// TODO: use <stack>

	Widget *w = nullptr;
	// Coordinates will be translated to this widget's parent
	Point origin;
	Point o_origin;
	Widget *current_widget = nullptr;
	PainterContext* context = nullptr;
	Pen *p = nullptr;
	Pen *o_p = nullptr;
	Brush *b = nullptr;
	Brush *o_b = nullptr;
	bool clipped = false;
	std::unique_ptr<Pen> def_pen;
	std::unique_ptr<Brush> def_brush;
	bool begun = false;
	// TODO: maybe extend this to a gMargins? and do all sides?
	int top_margin = 0; // for window TopBar.. 

	friend class Widget;
};

using UniquePainter = std::unique_ptr<Painter>;
using SharedPainter = std::shared_ptr<Painter>;

NAMESPACE_END
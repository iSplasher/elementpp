#pragma once

#include "element/widget.h"

NAMESPACE_BEGIN

class Painter;

class ELEMENT_API Pen {
public:
	enum class Cap;
	enum class Join;

	Pen() = default;
	explicit Pen(Painter &painter);

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

	PainterContext* pc = nullptr;
	float c_width = 0.1f;
	Color c_color;

	friend class Painter;
};

class ELEMENT_API Brush {
public:
	Brush() = default;
	explicit Brush(Painter &painter);

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
	void begin(float pixel_ratio=1);

	/// <summary>
	/// End painting
	/// </summary>
	void end();

	/// <summary>
	/// Set new pen
	/// </summary>
	/// <param name="pen">New pen</param>
	void setPen(Pen &pen);

	/// <summary>
	/// Retrieve current pen
	/// </summary>
	/// <returns>Current Pen</returns>
	Pen &pen() const { return *p; }

	/// <summary>
	/// Saves the current state
	/// </summary>
	/// <remarks>A matching restore() call must be used</remarks>
	void save();

	/// <summary>
	/// Restores saved state
	/// </summary>
	/// <remarks>A save() must have been called before calling this method</remarks>
	void restore();

	/// <summary>
	/// Resets to default state
	/// </summary>
	/// <remarks>This does not affect the state stack</remarks>
	void reset();

	/// <summary>
	/// Sets the transparency applied to all rendered shapes.
	/// Already transparent paths will get proportionally more transparent as well.
	/// </summary>
	/// <param name="alpha">new alpha value between 0 and 1</param>
	void setGlobalAlpha(float alpha) const;

	/// <summary>
	/// Set new brush
	/// </summary>
	/// <param name="brush">New brush</param>
	void setBrush(Brush &brush);

	/// <summary>
	/// Retrieve current brush
	/// </summary>
	/// <returns>Current Brush</returns>
	Brush &brush() const { return *b; }

	/// <summary>
	/// Draw a rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	void drawRect(RectF rect) const;
	void drawRect(Rect rect) const { drawRect(RectF(rect)); }

	/// <summary>
	/// Draw a rounded rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	/// <param name="radius">Radius of the rect corners</param>
	void drawRoundedRect(RectF rect, float radius) const;
	void drawRoundedRect(Rect rect, int radius) const { drawRoundedRect(RectF(rect), radius); }

	/// <summary>
	/// Draw an ellipse shape
	/// </summary>
	/// <param name="center">Center of ellipse</param>
	/// <param name="size">Width and height of ellipse</param>
	void drawEllipse(PointF center, SizeF size) const;

	/// <summary>
	/// Draw a circle shape
	/// </summary>
	/// <param name="center">Center of circle</param>
	/// <param name="radius">Circle radius</param>
	void drawCircle(PointF center, float radius) const;

	/// <summary>
	/// Draw a line
	/// </summary>
	/// <param name="start">Start point of line</param>
	/// <param name="end">End point of line</param>
	void drawLine(PointF start, PointF end) const;

private:

	// helper methods
	void paintWidget(Widget* w);
	void translate(RectF &r) const;
	void translate(PointF &p) const;
	void beginPath() const;
	void applyPB() const;

	// TODO: Create a save-stack struct to store old objects instead!

	Widget *w = nullptr;
	// Coordinates will be translated to this widget's parent
	PointF origin;
	PointF o_origin;
	Widget *current_widget = nullptr;
	PainterContext* context = nullptr;
	Pen *p = nullptr;
	Pen *o_p = nullptr;
	Brush *b = nullptr;
	Brush *o_b = nullptr;
	bool begun = false;
	// TODO: maybe extend this to a gMargins? and do all sides?
	int top_margin = 0; // for window TopBar.. 

	friend class Widget;
};

using UniquePainter = std::unique_ptr<Painter>;
using SharedPainter = std::shared_ptr<Painter>;

NAMESPACE_END
#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class Painter;

class GSPLASHER_API gPen {
public:
	enum class Cap;
	enum class Join;

	gPen() = default;
	explicit gPen(Painter &painter);

	/// <summary>
	/// Set a new color to this pen
	/// </summary>
	void setColor(gColor color);

	gColor color() const { return c_color; }

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
	gColor c_color;

	friend class Painter;
};

class GSPLASHER_API gBrush {
public:
	gBrush() = default;
	explicit gBrush(Painter &painter);

	/// <summary>
	/// Set a new color to this brush
	/// </summary>
	void setColor(gColor color);

	gColor color() const { return c_color; }

	//void setGradient();
	//void setPattern();

private:
	/// <summary>
	/// Applies the current brush style
	/// </summary>
	void apply() const;

	PainterContext* pc = nullptr;
	gColor c_color;

	friend class Painter;
};

class gTopBar;

class GSPLASHER_API Painter {
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
	void setPen(gPen &pen);

	/// <summary>
	/// Retrieve current pen
	/// </summary>
	/// <returns>Current gPen</returns>
	gPen &pen() const { return *p; }

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
	void setBrush(gBrush &brush);

	/// <summary>
	/// Retrieve current brush
	/// </summary>
	/// <returns>Current gBrush</returns>
	gBrush &brush() const { return *b; }

	/// <summary>
	/// Draw a rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	void drawRect(gRectF rect) const;
	void drawRect(gRect rect) const { drawRect(gRectF(rect)); }

	/// <summary>
	/// Draw a rounded rectangle shape
	/// </summary>
	/// <param name="rect">Shape dimensions</param>
	/// <param name="radius">Radius of the rect corners</param>
	void drawRoundedRect(gRectF rect, float radius) const;
	void drawRoundedRect(gRect rect, int radius) const { drawRoundedRect(gRectF(rect), radius); }

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
	void translate(gRectF &r) const;
	void translate(PointF &p) const;
	void beginPath() const;
	void applyPB() const;

	// TODO: Create a save-stack struct to store old objects instead!

	WidgetCore *w = nullptr;
	// Coordinates will be translated to this widget's parent
	PointF origin;
	PointF o_origin;
	WidgetCore *current_widget = nullptr;
	PainterContext* context = nullptr;
	gPen *p = nullptr;
	gPen *o_p = nullptr;
	gBrush *b = nullptr;
	gBrush *o_b = nullptr;
	bool begun = false;
	// TODO: maybe extend this to a gMargins? and do all sides?
	int top_margin = 0; // for window TopBar.. 

	friend class WidgetCore;
	friend class gTopBar;
};

using UniquePainter = std::unique_ptr<Painter>;
using SharedPainter = std::shared_ptr<Painter>;

NAMESPACE_END
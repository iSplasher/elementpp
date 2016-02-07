#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class gPainter;

class GSPLASHER_API gPen {
public:
	enum class Cap;
	enum class Join;

	gPen() = default;

	/// <summary>
	/// Set a new color to this pen
	/// </summary>
	void setColor(float r, float g, float b, float a) const;

	//Color color() const { return c_color; }

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
	float c_width = 1;

	friend class gPainter;
};

class GSPLASHER_API gBrush {
public:
	gBrush() = default;

	/// <summary>
	/// Set a new color to this brush
	/// </summary>
	void setColor(float r, float g, float b, float a) const;

	//Color color() const { return c_color; }

	//void setGradient();
	//void setPattern();

private:
	/// <summary>
	/// Applies the current brush style
	/// </summary>
	void apply() const;

	PainterContext* pc = nullptr;

	friend class gPainter;
};

class GSPLASHER_API gPainter {
public:
	gPainter(gCoreWidget*);
	~gPainter();

	/// <summary>
	/// Begin painting
	/// </summary>
	void begin() const;

	/// <summary>
	/// End painting
	/// </summary>
	void end() const;

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
	/// <param name="rect">A gReftF</param>
	void drawRect(gRectF rect) const;
	void drawRect(gRect rect) const { drawRect(gRectF(rect)); }

	/// <summary>
	/// Draw an ellipse shape
	/// </summary>
	/// <param name="center">Center of ellipse</param>
	/// <param name="radius"></param>
	void drawEllipse(gPointF center, gPointF radius) const;

	/// <summary>
	/// Draw a circle shape
	/// </summary>
	/// <param name="center">Center of circle</param>
	/// <param name="radius">Circle radius</param>
	void drawCircle(gPointF center, float radius) const;


private:

	// helper methods

	void beginPath() const;
	void applyPB() const;

	gCoreWidget *w = nullptr;
	gPen *p = nullptr;
	gBrush *b = nullptr;

};

NAMESPACE_END
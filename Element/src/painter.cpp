#include "element/core/painter.h"
#include "element/window.h"
#include "element/core/primitive.h"

#include <GL/glew.h> // required for nanovg
#include <GLFW/glfw3.h> // required for nanovg

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"

USING_NAMESPACE

_PColor Color::toPColor() const {
	NVGcolor c;
	switch (type) {
	case RGB:
		c = nvgRGB(red, green, blue);
		break;
	case RGBA:
		c = nvgRGBA(red, green, blue, alpha);
		break;
	case HSL:
		c = nvgHSL(hue, sat, light);
		break;
	case HSLA:
		c = nvgHSLA(hue, sat, light, alpha);
	}

	return c;
}

enum class Pen::Cap {
	Butt = NVG_BUTT,
	Round = NVG_ROUND,
	Square = NVG_SQUARE
};

enum class Pen::Join {
	Miter = NVG_MITER,
	Round = NVG_ROUND,
	Bevel = NVG_BEVEL
};

Pen::Pen(Painter& painter) {
	painter.setPen(*this);
	setJoin(Join::Bevel);
	setCap(Cap::Round);
	setColor(Color(0, 0, 0));
	setWidth(1);
}

void Pen::setColor(Color color) {
	if (pc) {
		nvgStrokeColor(pc, color.toPColor());
		c_color = color;
	}
}

void Pen::setWidth(float width) {
	if (pc) {
		nvgStrokeWidth(pc, width);
		c_width = width;
	}
}

void Pen::setCap(Cap cap) {
	if (pc) {
		nvgLineCap(pc, static_cast<int>(cap));
	}
}

void Pen::setJoin(Join join) {
	if (pc) {
		nvgLineJoin(pc, static_cast<int>(join));
	}
}

void Pen::apply() const {
	if (pc) {
		nvgStroke(pc);
	}
}

Brush::Brush(Painter& painter) {
	painter.setBrush(*this);
	setColor(Color(0, 0, 0));
}

void Brush::setColor(Color color) {
	if (pc) {
		nvgFillColor(pc, color.toPColor());
		c_color = color;
	}
}

void Brush::apply() const {
	if (pc) {
		nvgFill(pc);
	}
}

Painter::Painter(Window* window) {
	w = window;
	if (!w->this_paint) {
		w->this_paint = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
		if (!w->this_paint) {
			std::cout << "Fatal: Could not create paint context\n";
		}
	}
	context = w->this_paint;
	current_widget = window;
}

Painter::~Painter() {
}

const Painter& Painter::begin(float pixel_ratio) {
	if (begun) {
		throw std::runtime_error("Inconsistent Painter::begin call");
	}
	begun = true;
	w->parent_window->setActive();
	SizeF s = w->size;
	nvgBeginFrame(context, s.width, s.height, pixel_ratio);
	return *this;
}

const Painter& Painter::end() {
	if (!begun) {
		throw std::runtime_error("Inconsistent Painter::end call");
	}
	begun = false;
	nvgEndFrame(context);
	return *this;
}

const Painter& Painter::setPen(Pen& pen) {
	pen.pc = context;
	p = &pen;
	return *this;
}

const Painter& Painter::save() {
	nvgSave(context);
	o_origin = origin;
	o_p = p;
	o_b = b;
	return *this;
}

const Painter& Painter::restore() {
	nvgRestore(context);
	origin = o_origin;
	p = o_p;
	b = o_b;
	return *this;
}

const Painter& Painter::reset() {
	nvgReset(context);
	o_origin = PointF();
	origin = PointF();
	o_b = nullptr;
	b = nullptr;
	o_p = nullptr;
	p = nullptr;
	return *this;
}

const Painter& Painter::setGlobalAlpha(float alpha) const {
	nvgGlobalAlpha(context, alpha);
	return *this;
}

const Painter& Painter::setBrush(Brush& brush) {
	brush.pc = context;
	b = &brush;
	return *this;
}

const Painter& Painter::drawRect(RectF rect) const {
	beginPath();
	translate(rect);
	nvgRect(context, rect.x, rect.y, rect.width, rect.height);
	applyPB();
	return *this;
}

const Painter& Painter::drawRoundedRect(RectF rect, float radius) const {
	beginPath();
	translate(rect);
	nvgRoundedRect(context, rect.x, rect.y, rect.width, rect.height, radius);
	applyPB();
	return *this;
}

const Painter& Painter::drawRoundedRect( RectF rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left ) const {
	beginPath();
	translate(rect);
	nvgRoundedRectVarying(context, rect.x, rect.y, rect.width, rect.height, rad_top_left, rad_top_right,  rad_bottom_right, rad_bottom_left);
	applyPB();
	return *this;
}

const Painter& Painter::drawEllipse(PointF center, SizeF size) const {
	beginPath();
	translate(center);
	nvgEllipse(context, center.x, center.y, size.width, size.height);
	applyPB();
	return *this;
}

const Painter& Painter::drawCircle(PointF center, float radius) const {
	beginPath();
	translate(center);
	nvgCircle(context, center.x, center.y, radius);
	applyPB();
	return *this;
}

const Painter& Painter::drawLine(PointF start, PointF end) const {
	beginPath();
	translate(start);
	translate(end);
	nvgMoveTo(context, start.x, start.y);
	nvgLineTo(context, end.x, end.y);
	applyPB();

	return *this;
}

void Painter::paintWidget( Widget* w ) {
	Brush b(*this);
	Pen p(*this);
	b.setColor(w->backgroundColor);
	p.setColor(w->foregroundColor);
	RectF geometry = w->geometry;
	// draw border
	auto borderGeometry = RectF(geometry.x-w->borderLeft, geometry.y-w->borderTop, geometry.width+w->borderRight, geometry.height+w->borderBottom);

	// content
		drawRect(RectF(PointF(0, 0), w->size));
}

void Painter::translate(RectF& r) const {
	if (!(current_widget->type == ElementType::Window)) {
		r += origin;
		r.y += top_margin;
	}
}

void Painter::translate(PointF& p) const {
	if (!(current_widget->type == ElementType::Window)) {
		p += origin;
		p.y += top_margin;
	}
}

void Painter::beginPath() const {
	nvgBeginPath(context);
}

void Painter::applyPB() const {
	if (p) {
		p->apply();
	}
	if (b) {
		b->apply();
	}
}

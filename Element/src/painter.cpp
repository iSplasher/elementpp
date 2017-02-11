#include "element/utils/painter.h"
#include "element/window.h"
#include "element/utils/primitives.h"

#include <GL/glew.h> // required for nanovg
#include <GLFW/glfw3.h> // required for nanovg

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"

USING_NAMESPACE

_PColor gColor::toPColor() const {
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

enum class gPen::Cap {
	Butt = NVG_BUTT,
	Round = NVG_ROUND,
	Square = NVG_SQUARE
};

enum class gPen::Join {
	Miter = NVG_MITER,
	Round = NVG_ROUND,
	Bevel = NVG_BEVEL
};

gPen::gPen(Painter& painter) {
	painter.setPen(*this);
	setJoin(Join::Bevel);
	setCap(Cap::Round);
	setColor(gColor(0, 0, 0));
	setWidth(1);
}

void gPen::setColor(gColor color) {
	if (pc) {
		nvgStrokeColor(pc, color.toPColor());
		c_color = color;
	}
}

void gPen::setWidth(float width) {
	if (pc) {
		nvgStrokeWidth(pc, width);
		c_width = width;
	}
}

void gPen::setCap(Cap cap) {
	if (pc) {
		nvgLineCap(pc, static_cast<int>(cap));
	}
}

void gPen::setJoin(Join join) {
	if (pc) {
		nvgLineJoin(pc, static_cast<int>(join));
	}
}

void gPen::apply() const {
	if (pc) {
		nvgStroke(pc);
	}
}

gBrush::gBrush(Painter& painter) {
	painter.setBrush(*this);
	setColor(gColor(0, 0, 0));
}

void gBrush::setColor(gColor color) {
	if (pc) {
		nvgFillColor(pc, color.toPColor());
		c_color = color;
	}
}

void gBrush::apply() const {
	if (pc) {
		nvgFill(pc);
	}
}

gsp::Painter::Painter(RWindow* window) {
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

void Painter::begin(float pixel_ratio) {
	if (begun) {
		throw std::runtime_error("Inconsistent Painter::begin call");
	}
	begun = true;
	w->parent_window->setActive();
	if (w->parent_window->top_bar) {
		top_margin = w->parent_window->top_bar->size().height;
	}
	auto s = w->size();
	nvgBeginFrame(context, s.width, s.height, pixel_ratio);
}

void Painter::end() {
	if (!begun) {
		throw std::runtime_error("Inconsistent Painter::end call");
	}
	begun = false;
	nvgEndFrame(context);
}

void Painter::setPen(gPen& pen) {
	pen.pc = context;
	p = &pen;
}

void Painter::save() {
	nvgSave(context);
	o_origin = origin;
	o_p = p;
	o_b = b;
}

void Painter::restore() {
	nvgRestore(context);
	origin = o_origin;
	p = o_p;
	b = o_b;
}

void Painter::reset() {
	nvgReset(context);
	o_origin = PointF();
	origin = PointF();
	o_b = nullptr;
	b = nullptr;
	o_p = nullptr;
	p = nullptr;
}

void Painter::setGlobalAlpha(float alpha) const {
	nvgGlobalAlpha(context, alpha);
}

void Painter::setBrush(gBrush& brush) {
	brush.pc = context;
	b = &brush;
}

void Painter::drawRect(RectF rect) const {
	beginPath();
	translate(rect);
	nvgRect(context, rect.x, rect.y, rect.width, rect.height);
	applyPB();
}

void Painter::drawRoundedRect(RectF rect, float radius) const {
	beginPath();
	translate(rect);
	nvgRoundedRect(context, rect.x, rect.y, rect.width, rect.height, radius);
	applyPB();
}

void Painter::drawEllipse(PointF center, SizeF size) const {
	beginPath();
	translate(center);
	nvgEllipse(context, center.x, center.y, size.width, size.height);
	applyPB();
}

void Painter::drawCircle(PointF center, float radius) const {
	beginPath();
	translate(center);
	nvgCircle(context, center.x, center.y, radius);
	applyPB();
}

void Painter::drawLine(PointF start, PointF end) const {
	beginPath();
	translate(start);
	translate(end);
	nvgMoveTo(context, start.x, start.y);
	nvgLineTo(context, end.x, end.y);
	applyPB();
}

void Painter::translate(RectF& r) const {
	if (!current_widget->is_window) {
		r += origin;
		r.y += top_margin;
	}
}

void Painter::translate(PointF& p) const {
	if (!current_widget->is_window) {
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

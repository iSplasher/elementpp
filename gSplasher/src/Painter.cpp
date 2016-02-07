#include "gSplasher/Utils/Painter.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

gPen::gPen(gPainter& painter) {
	painter.setPen(*this);
	setJoin(Join::Bevel);
	setCap(Cap::Round);
	setColor(gColor(255, 255, 255, 255));
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

gsp::gPainter::gPainter(gCoreWidget* widget) {
	w = widget;
	if (!w->this_paint) {
		w->this_paint = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
		if (!w->this_paint) {
			std::cout << "Fatal: Could not create paint context\n";
		}
	}
	context = w->this_paint;
}

gPainter::~gPainter() {
}

void gPainter::begin() const {
	w->parent_window->setActive();
	int fb_width;
	int fb_height;
	auto s = w->size();

	glfwGetFramebufferSize(w->parent_window->r_window, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);

	float px_ratio = static_cast<float>(fb_width) / static_cast<float>(s.width);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(context, s.width, s.height, px_ratio);
}

void gPainter::end() const {
	nvgEndFrame(context);
}

void gPainter::setPen(gPen& pen) {
	pen.pc = context;
	p = &pen;
}

void gPainter::setBrush(gBrush& brush) {
	brush.pc = context;
	b = &brush;
}

void gPainter::drawRect(gRectF rect) const {
	beginPath();
	nvgRect(context, rect.x, rect.y, rect.width, rect.height);
	applyPB();
}

void gPainter::drawEllipse(gPointF center, gSizeF size) const {
	beginPath();
	nvgEllipse(context, center.x, center.y, size.width, size.height);
	applyPB();
}

void gPainter::drawCircle(gPointF center, float radius) const {
	beginPath();
	nvgCircle(context, center.x, center.y, radius);
	applyPB();
}

void gPainter::beginPath() const {
	nvgBeginPath(context);
}

void gPainter::applyPB() const {
	if (p) {
		p->apply();
	}
	if (b) {
		b->apply();
	}
}

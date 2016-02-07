#include "gSplasher/Utils/Painter.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"

USING_NAMESPACE

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

void gPen::setColor(float r, float g, float b, float a) const {
	if (pc) {
		auto c = nvgRGBAf(r, g, b, a);
		nvgStrokeColor(pc, c);
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

void gBrush::setColor(float r, float g, float b, float a) const {
	if (pc) {
		auto c = nvgRGBAf(r, g, b, a);
		nvgFillColor(pc, c);
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
		w->this_paint = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		if (!w->this_paint) {
			std::cout << "Fatal: Could not create paint context\n";
		}
	}
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

	float px_ratio = fb_width / fb_height;
	glClearColor(0, 0, 0, 50);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(w->this_paint, s.width, s.height, px_ratio);
}

void gPainter::end() const {
	nvgEndFrame(w->this_paint);
}

void gPainter::setPen(gPen& pen) {
	pen.pc = w->this_paint;
	p = &pen;
}

void gPainter::setBrush(gBrush& brush) {
	brush.pc = w->this_paint;
	b = &brush;
}

void gPainter::drawRect(gRectF rect) const {
	beginPath();
	nvgRect(w->this_paint, rect.x, rect.y, rect.width, rect.height);
	applyPB();
}

void gPainter::drawEllipse(gPointF center, gPointF radius) const {
	beginPath();
	nvgEllipse(w->this_paint, center.x, center.y, radius.x, radius.y);
	applyPB();
}

void gPainter::drawCircle(gPointF center, float radius) const {
	beginPath();
	nvgCircle(w->this_paint, center.x, center.y, radius);
	applyPB();
}

void gPainter::beginPath() const {
	nvgBeginPath(w->this_paint);
}

void gPainter::applyPB() const {
	if (p && b) {
		p->apply();
		b->apply();
	}
}

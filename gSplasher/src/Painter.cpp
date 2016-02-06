#include "gSplasher/Utils/Painter.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"

USING_NAMESPACE

gsp::gPainter::gPainter(gCoreWidget* widget) {
	w = widget;
	if (!w->this_paint) {
		w->this_paint = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		if (!w->this_paint) {
			std::cout << "Fatal: Could not create Nano Context\n";
		}
	}
}

void gPainter::begin() const {
	w->parent_window->setActive();
	int fb_width;
	int fb_height;
	auto s = w->size();
	auto p = w->pos();

	glfwGetFramebufferSize(w->parent_window->r_window, &fb_width, &fb_height);

	glViewport(p.x, p.y, fb_width, fb_height);

	float px_ratio = fb_width / fb_height;
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(w->this_paint, s.width, s.height, px_ratio);
}

void gPainter::end() const {
	nvgEndFrame(w->this_paint);
}

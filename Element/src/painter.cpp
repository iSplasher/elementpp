#include "element/core/painter.h"
#include "element/window.h"
#include "element/core/primitive.h"

#include <GL/glew.h> // required for nanovg
#include <GLFW/glfw3.h>

#include "deps/nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "deps/nanovg/nanovg_gl.h"

USING_NAMESPACE

_PColor Color::toPColor() const {
	NVGcolor c;
	switch( type ) {
		case RGB:
			c = nvgRGB( red, green, blue );
			break;
		case RGBA:
			c = nvgRGBA( red, green, blue, alpha );
			break;
		case HSL:
			c = nvgHSL( hue, sat, light );
			break;
		case HSLA:
			c = nvgHSLA( hue, sat, light, alpha );
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

Pen::Pen( Painter& painter, Color color ) {
	painter.setPen(*this);
	setJoin(Join::Bevel);
	setCap(Cap::Round);
	setColor(color);
	setWidth(1);
}

Pen::~Pen() {
	if (painter)
		painter->resetPen();
	
}

void Pen::setColor( Color color ) {
	if( pc ) {
		nvgStrokeColor( pc, color.toPColor() );
		c_color = color;
	}
}

void Pen::setWidth( float width ) {
	if( pc ) {
		nvgStrokeWidth( pc, width );
		c_width = width;
	}
}

void Pen::setCap( Cap cap ) {
	if( pc ) {
		nvgLineCap( pc, static_cast< int >( cap ) );
	}
}

void Pen::setJoin( Join join ) {
	if( pc ) {
		nvgLineJoin( pc, static_cast< int >( join ) );
	}
}

void Pen::apply() const {
	if( pc ) {
		nvgStroke( pc );
	}
}

Brush::Brush( Painter& painter, Color color ) {
	painter.setBrush(*this);
	setColor(Color(0, 0, 0));
}

Brush::~Brush() {
	if (painter)
		painter->resetBrush();
}

void Brush::setColor( Color color ) {
	if( pc ) {
		nvgFillColor( pc, color.toPColor() );
		c_color = color;
	}
}

void Brush::apply() const {
	if( pc ) {
		nvgFill( pc );
	}
}

Painter::Painter( Window* window ) :
	def_pen(std::make_unique<Pen>( *this )), def_brush( std::make_unique<Brush>( *this ) ){
	w = window;
	if( !w->this_paint ) {
		w->this_paint = nvgCreateGL3( NVG_STENCIL_STROKES | NVG_DEBUG );
		if( !w->this_paint ) {
			std::cout << "Fatal: Could not create paint context\n";
		}
	}
	context = w->this_paint;
	current_widget = window;
}

Painter::~Painter() {}

const Painter& Painter::begin( float pixel_ratio ) {
	if( begun ) {
		throw std::runtime_error( "Inconsistent Painter::begin call" );
	}
	begun = true;
	w->parent_window->setActive();
	Size s = w->size;
	nvgBeginFrame( context, s.width, s.height, pixel_ratio );
	return *this;
}

const Painter& Painter::end() {
	if( !begun ) {
		throw std::runtime_error( "Inconsistent Painter::end call" );
	}
	begun = false;
	nvgEndFrame( context );
	return *this;
}

const Painter& Painter::setPen( Pen& pen ) {
	pen.pc = context;
	pen.painter = this;
	p = &pen;
	return *this;
}

const Painter& Painter::save() {
	nvgSave( context );
	o_origin = origin;
	o_p = p;
	o_b = b;
	return *this;
}

const Painter& Painter::restore() {
	nvgRestore( context );
	origin = o_origin;
	p = o_p;
	b = o_b;
	return *this;
}

const Painter& Painter::reset() {
	nvgReset( context );
	o_origin = Point();
	origin = Point();
	o_b = nullptr;
	b = nullptr;
	o_p = nullptr;
	p = nullptr;
	return *this;
}

const Painter& Painter::resetPen() {
	setPen(*def_pen);
	return *this;
}

const Painter& Painter::resetBrush() {
	setBrush(*def_brush);
	return *this;
}

const Painter& Painter::setGlobalAlpha( float alpha ) const {
	nvgGlobalAlpha( context, alpha );
	return *this;
}

const Painter& Painter::setBrush( Brush& brush ) {
	brush.painter = this;
	brush.pc = context;
	b = &brush;
	return *this;
}

const Painter& Painter::clip( Rect rect ) const {
	translate( rect );
	nvgScissor(context, rect.x, rect.y, rect.width, rect.height);
	return *this;
}

const Painter& Painter::resetClip() const {
	nvgResetScissor(context);
	return *this;
}

const Painter& Painter::drawRect( Rect rect ) const {
	beginPath();
	translate( rect );
	nvgRect( context, rect.x, rect.y, rect.width, rect.height );
	applyPB();
	return *this;
}

const Painter& Painter::drawRoundedRect( Rect rect, float radius ) const {
	beginPath();
	translate( rect );
	nvgRoundedRect( context, rect.x, rect.y, rect.width, rect.height, radius );
	applyPB();
	return *this;
}

const Painter& Painter::drawRoundedRect( Rect rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left ) const {
	beginPath();
	translate( rect );
	nvgRoundedRectVarying( context, rect.x, rect.y, rect.width, rect.height, rad_top_left, rad_top_right, rad_bottom_right, rad_bottom_left );
	applyPB();
	return *this;
}

const Painter& Painter::drawEllipse( Point center, Size size ) const {
	beginPath();
	translate( center );
	nvgEllipse( context, center.x, center.y, size.width, size.height );
	applyPB();
	return *this;
}

const Painter& Painter::drawCircle( Point center, float radius ) const {
	beginPath();
	translate( center );
	nvgCircle( context, center.x, center.y, radius );
	applyPB();
	return *this;
}

const Painter& Painter::drawLine( Point start, Point end ) const {
	beginPath();
	translate( start );
	translate( end );
	nvgMoveTo( context, start.x, start.y );
	nvgLineTo( context, end.x, end.y );
	applyPB();

	return *this;
}

void Painter::paintWidget( Widget* w ) {
	Brush b( *this );
	Pen p( *this );
	p.setColor( w->foregroundColor );
	p.setWidth( 0 );
	Size size = w->size;
	b.setColor( w->borderColor );
	drawRoundedRect( Rect( 0, 0, size.width, size.height ) , w->borderRadiusTopLeft, w->borderRadiusTopRight, w->borderRadiusBottomRight, w->borderRadiusBottomLeft); // border
	b.setColor( w->backgroundColor );
	auto content_geometry = Rect( w->borderLeft, w->borderTop, size.width - w->borderRight * 2, size.height - w->borderBottom * 2 );
	drawRect( Rect( content_geometry ) ); // contents
	p.setWidth( 1 );
}

void Painter::translate( Rect& r ) const {
	if( !( current_widget->type == ElementType::Window ) ) {
		r += origin;
		r.y += top_margin;
	}
}

void Painter::translate( Point& p ) const {
	if( !( current_widget->type == ElementType::Window ) ) {
		p += origin;
		p.y += top_margin;
	}
}

void Painter::beginPath() const {
	nvgBeginPath( context );
}

void Painter::applyPB() const {
	if( p ) {
		p->apply();
	}
	if( b ) {
		b->apply();
	}
}

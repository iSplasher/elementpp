#include "element/widget.h"
#include "element/window.h"
#include "element/core/painter.h"


USING_NAMESPACE

Widget::Widget( Widget* parent ) : PRIV_NAMESPACE::Layoutable( parent ),
                                                 contentGeometry( [&](RectF n) -> RectF {
	                                                                  return RectF( n.x + paddingLeft + borderLeft,
	                                                                                n.y + paddingTop + borderLeft,
	                                                                                n.width - ( paddingRight + borderRight ) * 2,
	                                                                                n.height - ( paddingBottom + borderBottom ) * 2 );
                                                                  }, geometry ),
                                                 contentSize( [&](SizeF n) -> SizeF {
	                                                              return SizeF( n.width - ( paddingRight + borderRight ) * 2,
	                                                                            n.height - ( paddingBottom + borderBottom ) * 2 );
                                                              }, size ),
                                                 borderRadiusTopLeft( 0 ),
                                                 borderRadiusTopRight( 0 ),
                                                 borderRadiusBottomRight( 0 ),
                                                 borderRadiusBottomLeft( 0 ) {
	objectName = "Widget";
	setType( ElementType::Widget );

	marginLeft = marginTop = marginRight = marginBottom = 5;
	paddingLeft = paddingTop = paddingRight = paddingBottom = 5;
	borderLeft = borderTop = borderRight = borderBottom = 5;
	borderColor = Color( 27, 27, 27, 200 );
	backgroundColor = Color( 255, 0, 0 );
	foregroundColor = Color( 0, 0, 0 );
}

Widget::~Widget() {
	// TODO: delete paint context
}

void Widget::paint( Painter& painter ) {
	auto b = Brush( painter );
	b.setColor( Color( 255, 255, 255 ) );
	painter.drawRect( RectF( 0, 0, contentSize ) );
}

void Widget::update() {
	if( parent_window ) {
		auto& painter = *parent_window->painter;
		painter.save();
		painter.origin = PointF( mapToWindow( PointF( 0, 0 ) ) );
		painter.current_widget = this;
		if( paintWidget )
			painter.paintWidget( this );
		painter.origin = PointF( contentGeometry.get().pos() );
		painter.clip( RectF( 0, 0, contentSize ) );
		paint( painter );
		painter.restore();
	}
	Layoutable::update();
}

PointF Widget::mapToParent( PointF _p ) const {
	return _p + position;
}

PointF Widget::mapFromParent( PointF _p ) const {
	return _p - position;
}

PointF Widget::mapFromScreen( PointF p ) {
	auto w = this;

	while( w ) {
		p -= w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

PointF Widget::mapToScreen( PointF p ) {
	auto w = this;

	while( w ) {
		p += w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

void Widget::setParent( Element* e ) {
	Layoutable::setParent( e );
	parent_widget = static_cast< Widget* >( e );
	if( e && e->type == ElementType::Window )
		parent_window = static_cast< Window* >( parent_widget );
	else if( e )
		parent_window = parent_widget->parent_window;
}

PointF Widget::mapFromWindow( PointF p ) {
	if( type == ElementType::Window ) {
		return p;
	}
	auto w = this;

	while( w ) {
		p -= w->position;
		w = w->parent_widget && w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

PointF Widget::mapToWindow( PointF p ) {
	if( type == ElementType::Window ) {
		return p;
	}
	auto w = this;

	while( w ) {
		p += w->position;
		w = w->parent_widget && w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

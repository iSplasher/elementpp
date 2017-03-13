#include "element/widget.h"
#include "element/window.h"
#include "element/core/painter.h"


USING_NAMESPACE

Widget::Widget( Widget* parent ) : PRIV_NAMESPACE::Layoutable( parent ),
                                                 contentGeometry( [&](Rect n) -> Rect {
	                                                                  return Rect( n.x + paddingLeft + borderLeft,
	                                                                               n.y + paddingTop + borderLeft,
	                                                                               n.width - ( paddingRight + borderRight ) * 2,
	                                                                               n.height - ( paddingBottom + borderBottom ) * 2 );
                                                                  }, geometry ),
                                                 contentSize( [&](Size n) -> Size {
	                                                              return Size( n.width - ( paddingRight + borderRight ) * 2,
	                                                                           n.height - ( paddingBottom + borderBottom ) * 2 );
                                                              }, size ),
                                                 borderRadiusTopLeft( 0 ),
                                                 borderRadiusTopRight( 0 ),
                                                 borderRadiusBottomLeft( 0 ),
                                                 borderRadiusBottomRight( 0 ) {
	objectName = "Widget";
	setType( ElementType::Widget );

	mouseMoved.changed( handleMove );
	position.changed( [&](Point p) {
		                 if( this->type != ElementType::Window )
			                 this->moved = p;
	                 } );
	leftPress.changed( [&](Point p) {
		                  this->last_mouse_pos = p;
	                  } );

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

void Widget::paint( Painter& painter ) {}

void Widget::update() {
	if( parent_window ) {
		auto& painter = *parent_window->painter;
		painter.save();
		painter.origin = Point( mapToWindow( Point( 0, 0 ) ) );
		painter.current_widget = this;
		if( paintWidget )
			painter.paintWidget( this );
		painter.origin = Point( contentGeometry.get().pos() );
		painter.clip( Rect( 0, 0, contentSize ) );
		paint( painter );
		painter.restore();
	}
	Layoutable::update();
}

Point Widget::mapToParent( Point _p ) const {
	return _p + position;
}

Point Widget::mapFromParent( Point _p ) const {
	return _p - position;
}

Point Widget::mapFromScreen( Point p ) {
	auto w = this;

	while( w ) {
		p -= w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

Point Widget::mapToScreen( Point p ) {
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

void Widget::handleMove( MouseEvent m_ev ) {
	auto w = m_ev.widget;
	if( w->isDraggable && !w->is_resizing ) {
		// check if left button is pressed
		if( flags( m_ev.button & MouseButton::Left ) ) {

			auto curr_m = w->mapToScreen( m_ev.position );
			w->position = curr_m - w->last_mouse_pos;
		}
	}
}

void Widget::windowMovedHelper( Point p ) {
	moved = p;
}

Point Widget::mapFromWindow( Point p ) {
	if( type == ElementType::Window ) {
		return p;
	}
	auto w = this;

	while( w ) {
		p -= w->position;
		w = w->parent_widget && w->parent_widget->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

Point Widget::mapToWindow( Point p ) {
	if( type == ElementType::Window ) {
		return p;
	}
	auto w = this;

	while( w ) {
		p += w->position;
		w = w->parent_widget && w->parent_widget->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

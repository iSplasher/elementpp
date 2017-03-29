#include "element/widget.h"
#include "element/window.h"
#include "element/core/painter.h"


USING_NAMESPACE

Widget::Widget( Widget* parent ) : Layoutable( parent ),
                                   contentGeometry( [&](Rect n) -> Rect {
	                                                    return Rect( n.x + paddingLeft + borderLeft,
	                                                                 n.y + paddingTop + borderLeft,
	                                                                 n.width - ( paddingRight + borderRight ) * 2,
	                                                                 n.height - ( paddingBottom + borderBottom ) * 2 );
                                                    }, geometry ),
                                   contentSize( [&](Size n) -> Size {
	                                                return Size( n.width - ( paddingRight + borderRight ) * 2,
	                                                             n.height - ( paddingBottom + borderBottom ) * 2);
                                                }, size ),
                                   borderRadiusTopLeft( 0 ),
                                   borderRadiusTopRight( 0 ),
                                   borderRadiusBottomLeft( 0 ),
                                   borderRadiusBottomRight( 0 ) {
	this->parent = parent; // we've overriden setParent so we need to call from this class too
	objectName = "Widget";
	if( parent ) {
		parent->append( this );
	}
	setType( ElementType::Widget );

	mouseMoved.changed( handleMove );
	position.changed( [&](Point p) {
		                 if( this->type != ElementType::Window )
			                 this->moved = p;
	                 } );
	leftPress.changed( [&](Point p) {
		                  this->last_mouse_pos = p;
	                  } );
	size.changed( [&](Size s) { resized = Rect( position, s ); } );

	borderColor = Color( 232, 23, 94, 200 );
	backgroundColor = Color( 72, 72, 72, 255 );
	foregroundColor = Color( 0, 0, 0 );
}

Widget::~Widget() {
	// TODO: delete paint context
}

void Widget::paint( Painter& painter ) {}

void Widget::update() {
	if( parent_window ) {
		auto& painter = *parent_window->painter;
		if( paintWidget && parent_window ) {
			painter.save();
			painter.origin = Point( mapToWindow( Point( 0, 0 ) ) );
			painter.current_widget = this;
			painter.paintWidget( this );
			painter.origin = Point( contentGeometry.get().pos() );
			painter.clip( Rect( 0, 0, contentSize ) );
			paint( painter );
			painter.restore();
		}
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

Direction Widget::hitTest( Point p ) {
	// skip if point is outside
	auto g = geometry.get();
	if (type == ElementType::Window) {
		g.x = 0;
		g.y = 0;
	}
	if (!g.contains(p)) {
		return Direction::None;
	}
	// skip if widget is hidden
	if (!visible.get()) {
		return Direction::None;
	}

	auto d = inResizeRangeHelper(p);
	if (d == Direction::None) {
		for (auto x : children()) {
			auto w = static_cast<Widget*>(x);
			auto d_c = w->hitTest(p);
			if (d_c != Direction::None) {
				return d_c;
			}
		}
	}

	if (isDraggable)
		return Direction::Default;
	return d;
}

void Widget::setParent( Element* e ) {
	if( !e ) {
		if( parent.get() )
			static_cast< Widget* >( parent.get() )->take( this );
	}

	Layoutable::setParent( e );
	parent_widget = e ? static_cast< Widget* >( e ) : nullptr;
	if( e && e->type == ElementType::Window )
		parent_window = static_cast< Window* >( parent_widget );
	else if( e )
		parent_window = parent_widget->parent_window;

	if( e ) {
		static_cast< Widget* >( e )->append( this );
	}
}

void Widget::handleMove( MouseEvent m_ev ) {
	auto w = m_ev.widget;
	if( w->isDraggable && !w->parent_window->grabbed_widget ) {
		// check if left button is pressed
		if( flags( m_ev.button & MouseButton::Left ) ) {

			auto curr_m = w->mapToScreen( m_ev.position );
			w->position = curr_m - w->last_mouse_pos;
		}
	}
}

Direction Widget::inResizeRangeHelper( Point p ) {
	Direction dir = Direction::None;
	if (!isResizeable)
		return dir;
	auto r = geometry.get();
	if (type == ElementType::Window) {
		r = Point();
	}
	auto resize_range = 6.0f;
	if ((p.x - r.x) < resize_range)
		dir = Direction::Left;
	else if ((r.x + r.width - p.x) < resize_range)
		dir = Direction::Right;
	else if ((p.y - r.y) < resize_range)
		dir = Direction::Top;
	else if ((r.y + r.height - p.y) < resize_range)
		dir = Direction::Bottom;

	return dir;
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

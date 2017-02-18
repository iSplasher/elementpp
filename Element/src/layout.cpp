#include "element/layout.h"
#include "element/widget.h"

#include <yoga/Yoga.h>

#include <cstdlib>


USING_NAMESPACE
USING_NAMESPACE_PRIV

YGFlexDirection getOrientation( Orientation o, bool reverse ) {
	switch( o ) {
		case Orientation::Horizontal:
			return reverse ? YGFlexDirectionRowReverse : YGFlexDirectionRow;
		case Orientation::Vertical:
			return reverse ? YGFlexDirectionColumnReverse : YGFlexDirectionColumn;
	}
}

Layoutable::Layoutable( Layoutable* parent ) : Element( parent ),
                                               geometry( [](PointF pos, SizeF size) -> RectF {
	                                                         return RectF( pos, size );
                                                         }, position, size ),
                                               marginLeft( 0 ),
                                               marginTop( 0 ),
                                               marginRight( 0 ),
                                               marginBottom( 0 ),
                                               borderLeft( 0 ),
                                               borderTop( 0 ),
                                               borderRight( 0 ),
                                               borderBottom( 0 ),
                                               paddingLeft( 0 ),
                                               paddingTop( 0 ),
                                               paddingRight( 0 ),
                                               paddingBottom( 0 ),
                                               alignment( Alignment::Default ),
                                               growth( 0 ),
                                               absolutePosition( false ),
                                               layout( this, std::mem_fn( &Layoutable::getLayout ) ) {

	if( parent && parent->type == ElementType::Layout ) {
		auto l = static_cast< Layout* >( parent );
		l->append( this );
	}

	// connect properties
	position.changed( [&](PointF n) {
		                 if( node ) {
			                 YGNodeStyleSetPosition( node, YGEdgeLeft, n.x );
			                 YGNodeStyleSetPosition( node, YGEdgeTop, n.y );
			                 if( !calculating )
				                 dirty_layout = true;
		                 }
	                 } );
	size.changed( [&](SizeF n) {
		             if( node ) {
			             YGNodeStyleSetWidth( node, n.width );
			             YGNodeStyleSetHeight( node, n.height );
			             if( !calculating )
				             dirty_layout = true;
		             }
	             } );
	minSize.changed( [&](SizeF n) {
		                if( node ) {
			                YGNodeStyleSetMinWidth( node, n.width );
			                YGNodeStyleSetMinHeight( node, n.height );
			                if( !calculating )
				                dirty_layout = true;
		                }
	                } );
	maxSize.changed( [&](SizeF n) {
		                if( node ) {
			                YGNodeStyleSetMaxWidth( node, n.width );
			                YGNodeStyleSetMaxHeight( node, n.height );
			                if( !calculating )
				                dirty_layout = true;
		                }
	                } );

	marginLeft.changed( [&](float n) {
		                   if( node ) {
			                   YGNodeStyleSetMargin( node, YGEdgeLeft, n );
			                   if( !calculating )
				                   dirty_layout = true;
		                   }
	                   } );
	marginTop.changed( [&](float n) {
		                  if( node ) {
			                  YGNodeStyleSetMargin( node, YGEdgeTop, n );
			                  if( !calculating )
				                  dirty_layout = true;
		                  }
	                  } );
	marginRight.changed( [&](float n) {
		                    if( node ) {
			                    YGNodeStyleSetMargin( node, YGEdgeRight, n );
			                    if( !calculating )
				                    dirty_layout = true;
		                    }
	                    } );
	marginBottom.changed( [&](float n) {
		                     if( node ) {
			                     YGNodeStyleSetMargin( node, YGEdgeBottom, n );
			                     if( !calculating )
				                     dirty_layout = true;
		                     }
	                     } );

	borderLeft.changed( [&](float n) {
		                   if( node ) {
			                   YGNodeStyleSetBorder( node, YGEdgeLeft, n );
			                   if( !calculating )
				                   dirty_layout = true;
		                   }
	                   } );
	borderTop.changed( [&](float n) {
		                  if( node ) {
			                  YGNodeStyleSetBorder( node, YGEdgeTop, n );
			                  if( !calculating )
				                  dirty_layout = true;
		                  }
	                  } );
	borderRight.changed( [&](float n) {
		                    if( node ) {
			                    YGNodeStyleSetBorder( node, YGEdgeRight, n );
			                    if( !calculating )
				                    dirty_layout = true;
		                    }
	                    } );
	borderBottom.changed( [&](float n) {
		                     if( node ) {
			                     YGNodeStyleSetBorder( node, YGEdgeBottom, n );
			                     if( !calculating )
				                     dirty_layout = true;
		                     }
	                     } );

	paddingLeft.changed( [&](float n) {
		                    if( node ) {
			                    YGNodeStyleSetPadding( node, YGEdgeLeft, n );
			                    if( !calculating )
				                    dirty_layout = true;
		                    }
	                    } );
	paddingTop.changed( [&](float n) {
		                   if( node ) {
			                   YGNodeStyleSetPadding( node, YGEdgeTop, n );
			                   if( !calculating )
				                   dirty_layout = true;
		                   }
	                   } );
	paddingRight.changed( [&](float n) {
		                     if( node ) {
			                     YGNodeStyleSetPadding( node, YGEdgeRight, n );
			                     if( !calculating )
				                     dirty_layout = true;
		                     }
	                     } );
	paddingBottom.changed( [&](float n) {
		                      if( node ) {
			                      YGNodeStyleSetPadding( node, YGEdgeBottom, n );
			                      if( !calculating )
				                      dirty_layout = true;
		                      }
	                      } );

	growth.changed( [&](float n) {
		               if( node ) {

			               float g = 0, s = 0;
			               if( n > 0 )
				               g = n;
			               else if( n < 0 )
				               s = std::abs( n );
			               YGNodeStyleSetFlexGrow( node, g );
			               YGNodeStyleSetFlexShrink( node, s );
			               if( !calculating )
				               dirty_layout = true;
		               }
	               } );
	absolutePosition.changed( [&](bool n) {
		                         if( node ) {
			                         auto x = n ? YGPositionTypeAbsolute : YGPositionTypeRelative;
			                         YGNodeStyleSetPositionType( node, x );
			                         if( !calculating )
				                         dirty_layout = true;
		                         }
	                         } );

	// set default values
	size = SizeF( 200, 200 );

}

Layoutable::~Layoutable() {
	if (node)
		YGNodeFree(node);
}


void Layoutable::update() {
	updateChildren();
}

void Layoutable::updateChildren() {
	for( auto c : children() ) {
		static_cast< Layoutable* >( c )->update();
	}
}

void Layoutable::invalidated() {
	if( node && parent_has_calculated ) {
		position = PointF( YGNodeStyleGetPosition( node, YGEdgeLeft ).value, YGNodeStyleGetPosition( node, YGEdgeTop ).value );
		size = SizeF( YGNodeStyleGetWidth( node ).value, YGNodeStyleGetHeight( node ).value );
		minSize = SizeF( YGNodeStyleGetMinWidth( node ).value, YGNodeStyleGetMinHeight( node ).value );
		maxSize = SizeF( YGNodeStyleGetMaxWidth( node ).value, YGNodeStyleGetMaxHeight( node ).value );

		marginLeft = YGNodeStyleGetMargin( node, YGEdgeLeft ).value;
		marginTop = YGNodeStyleGetMargin( node, YGEdgeTop ).value;
		marginRight = YGNodeStyleGetMargin( node, YGEdgeRight ).value;
		marginBottom = YGNodeStyleGetMargin( node, YGEdgeBottom ).value;

		borderLeft = YGNodeStyleGetBorder( node, YGEdgeLeft );
		borderTop = YGNodeStyleGetBorder( node, YGEdgeTop );
		borderRight = YGNodeStyleGetBorder( node, YGEdgeRight );
		borderBottom = YGNodeStyleGetBorder( node, YGEdgeBottom );

		paddingLeft = YGNodeStyleGetPadding( node, YGEdgeLeft ).value;
		paddingTop = YGNodeStyleGetPadding( node, YGEdgeTop ).value;
		paddingRight = YGNodeStyleGetPadding( node, YGEdgeRight ).value;
		paddingBottom = YGNodeStyleGetPadding( node, YGEdgeBottom ).value;

		float g = YGNodeStyleGetFlexGrow( node ), s = YGNodeStyleGetFlexShrink( node );
		if( g && !s )
			growth = g;
		else if( s && !g )
			growth = -s;
		else {
			growth = 0; // TODO: find middle value og g and s
		}

	}
	dirty_layout = false;
}

void Layoutable::applyStyle() {
	// connect properties
			YGNodeStyleSetPosition(node, YGEdgeLeft, position.get().x);
			YGNodeStyleSetPosition(node, YGEdgeTop, position.get().y);

			YGNodeStyleSetWidth(node, size.get().width);
			YGNodeStyleSetHeight(node, size.get().height);

			YGNodeStyleSetMinWidth(node, minSize.get().width);
			YGNodeStyleSetMinHeight(node, minSize.get().height);

			YGNodeStyleSetMaxWidth(node, maxSize.get().width);
			YGNodeStyleSetMaxHeight(node, maxSize.get().height);

			YGNodeStyleSetMargin(node, YGEdgeLeft, marginLeft);
			YGNodeStyleSetMargin(node, YGEdgeTop, marginTop);
			YGNodeStyleSetMargin(node, YGEdgeRight, marginRight);
			YGNodeStyleSetMargin(node, YGEdgeBottom, marginBottom);

			YGNodeStyleSetBorder(node, YGEdgeLeft, borderLeft);
			YGNodeStyleSetBorder(node, YGEdgeTop, borderTop);
			YGNodeStyleSetBorder(node, YGEdgeRight, borderRight);
			YGNodeStyleSetBorder(node, YGEdgeBottom, borderBottom);

			YGNodeStyleSetPadding(node, YGEdgeLeft, paddingLeft);
			YGNodeStyleSetPadding(node, YGEdgeTop, paddingTop);
			YGNodeStyleSetPadding(node, YGEdgeRight, paddingRight);
			YGNodeStyleSetPadding(node, YGEdgeBottom, paddingBottom);

			float g = 0, s = 0;
			if (growth.get() > 0)
				g = growth;
			else if (growth.get() < 0)
				s = std::abs(growth);
			YGNodeStyleSetFlexGrow(node, g);
			YGNodeStyleSetFlexShrink(node, s);

			auto x = absolutePosition ? YGPositionTypeAbsolute : YGPositionTypeRelative;
			YGNodeStyleSetPositionType(node, x);
			if (!calculating)
				dirty_layout = true;
}

Layout* Layoutable::getLayout() const {
	return playout;
}

Layout::Layout() : Layoutable(),
                   position( [](PointF p)-> PointF { return p; }, Layoutable::position ),
                   size( [](SizeF s)-> SizeF { return s; }, Layoutable::size ),
                   orientation( Orientation::Default ),
                   wrap( false ) {

	setType( ElementType::Layout );
	node = YGNodeNew();
	objectName = "Layout";
}

//void Layout::setWigdet( Widget* new_parent ) {
//	if( !new_parent->layout() ) {
//		new_parent->playout = this;
//		this->parent = new_parent;
//		new_parent->bound_layout = this;
//		properties.size = new_parent->properties.size;
//		playout = this;
//	}
//	else {
//		std::cout << "This wigdet is already handled by a layout\n"; // TODO: log
//	}
//}

void Layout::append( Layoutable* item, Alignment align, float grow ) {
	auto l = item->layout.get();

	if( l == this )
		return; // TODO: maybe warn?

	// take from old layout

	if( l )
		l->take( item );

	if( !item->node ) {
		item->node = YGNodeNew();
	}

	auto n = YGNodeGetChildCount( node );
	YGNodeInsertChild( node, item->node, n );
	nodemap.insert( { item->node, item } );

	// take ownership of layout
	if( item->type == ElementType::Layout )
		item->parent = this;

	item->applyStyle();

	dirty_layout = true;
}

void Layout::append( std::initializer_list< priv::Layoutable* > item, Alignment align, float grow ) {
	for( auto& i : item ) {
		append( i, align, grow );
	}
}

void Layout::update() {
	if( dirty_layout ) {
		calculating = true;
		invalidate();
		calculating = false;
	}
	Layoutable::update();
}

void Layout::take( Layoutable* item ) {
	if( item->layout.get() == this ) {
		YGNodeRemoveChild( node, item->node );
		nodemap.erase( item->node );
		item->layout = nullptr;

		// remove layout ownership
		if( item->type == ElementType::Layout )
			item->parent = nullptr;

	}
}

void Layout::invalidate() {

	RectF r;

	if( _widget ) { // if this layout is managing a widget, use the widget's properties
		r = _widget->geometry;
	}
	else { // else, use our own properties
		r = geometry;
	}
	// apply new calculations
	invalidated();

	YGNodeCalculateLayout( node, r.width, r.height, YGDirectionLTR );


	// inform all underlying layouts to recalculate
	for( auto i : nodemap ) {
		i.second->parent_has_calculated = true;
		if( i.second->type == ElementType::Layout )
			static_cast< Layout* >( i.second )->invalidate();
		else
			i.second->invalidated();
	}
}

void Layout::invalidated() {
	Layoutable::invalidated();

	if( node && parent_has_calculated ) { }
}


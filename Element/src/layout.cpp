#include "element/layout.h"
#include "element/widget.h"
#include "deps/yoga/Yoga.h"

USING_NAMESPACE
USING_NAMESPACE_PRIV

Layoutable::Layoutable( Layoutable* parent ) : Element( parent ),
                                               geometry( [](Point pos, Size size) -> Rect {
	                                                         return Rect( pos, size );
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
                                               grow( 1 ),
                                               shrink( 0 ),
                                               absolutePosition( false ),
                                               layout( this, std::mem_fn( &Layoutable::getLayout ) ) {

	if( parent && parent->type == ElementType::Layout ) {
		auto l = static_cast< Layout* >( parent );
		l->append( this );
	}

	// connect properties
	position.changed( [&](Point n) {
		                 if( node ) {
			                 YGNodeStyleSetPosition( node, YGEdgeLeft, n.x );
			                 YGNodeStyleSetPosition( node, YGEdgeTop, n.y );
			                 if( !calculating )
				                 dirty_layout = true;
		                 }
	                 } );
	size.changed( [&](Size n) {
		             setSize( n );
	             } );
	minSize.changed( [&](Size n) {
		                setMinSize( n );
	                } );
	maxSize.changed( [&](Size n) {
		                setMaxSize( n );
	                } );

	marginLeft.changed( [&](float n) {
		                   if( node ) {
			                   setMargin( n, Direction::Left );
			                   if( !calculating )
				                   dirty_layout = true;
		                   }
	                   } );
	marginTop.changed( [&](float n) {
		                  if( node ) {
			                  setMargin( n, Direction::Top );
			                  if( !calculating )
				                  dirty_layout = true;
		                  }
	                  } );
	marginRight.changed( [&](float n) {
		                    if( node ) {
			                    setMargin( n, Direction::Right );
			                    if( !calculating )
				                    dirty_layout = true;
		                    }
	                    } );
	marginBottom.changed( [&](float n) {
		                     if( node ) {
			                     setMargin( n, Direction::Bottom );
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

	grow.changed( [&](float n) { setGrow( n ); } );
	shrink.changed( [&](float n) { setShrink( n ); } );

	absolutePosition.changed( [&](bool n) {
		                         if( node ) {
			                         auto x = n ? YGPositionTypeAbsolute : YGPositionTypeRelative;
			                         YGNodeStyleSetPositionType( node, x );
			                         if( !calculating )
				                         dirty_layout = true;
		                         }
	                         } );
	alignment.changed( [&](Alignment n) { setAlignment( n ); } );

}

Layoutable::~Layoutable() {
	if( node )
		YGNodeFree( node );
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
	if( node ) {
		position = Point( YGNodeLayoutGetLeft( node ), YGNodeLayoutGetTop( node ) );
		size = Size( YGNodeLayoutGetWidth( node ), YGNodeLayoutGetHeight( node ) );
		minSize = Size( YGNodeStyleGetMinWidth( node ).value, YGNodeStyleGetMinHeight( node ).value );
		maxSize = Size( YGNodeStyleGetMaxWidth( node ).value, YGNodeStyleGetMaxHeight( node ).value );

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

		grow = YGNodeStyleGetFlexGrow( node );
		shrink = YGNodeStyleGetFlexShrink( node );

	}
	dirty_layout = false;
}

void Layoutable::applyStyle() {
	if( !node )
		return;

	setPosition( position );
	setSize( size );
	setMaxSize( maxSize );
	setMinSize( minSize );

	setMargin( marginLeft, Direction::Left );
	setMargin( marginTop, Direction::Top );
	setMargin( marginRight, Direction::Right );
	setMargin( marginBottom, Direction::Bottom );

	YGNodeStyleSetBorder( node, YGEdgeLeft, borderLeft );
	YGNodeStyleSetBorder( node, YGEdgeTop, borderTop );
	YGNodeStyleSetBorder( node, YGEdgeRight, borderRight );
	YGNodeStyleSetBorder( node, YGEdgeBottom, borderBottom );

	YGNodeStyleSetPadding( node, YGEdgeLeft, paddingLeft );
	YGNodeStyleSetPadding( node, YGEdgeTop, paddingTop );
	YGNodeStyleSetPadding( node, YGEdgeRight, paddingRight );
	YGNodeStyleSetPadding( node, YGEdgeBottom, paddingBottom );

	setAlignment( alignment );
	setGrow( grow );
	setShrink( shrink );

	auto x = absolutePosition ? YGPositionTypeAbsolute : YGPositionTypeRelative;
	YGNodeStyleSetPositionType( node, x );
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setGrow( float n ) {
	if( node ) {
		if( n >= 0 )
			YGNodeStyleSetFlexGrow( node, n );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setShrink( float n ) {
	if( node ) {
		if( n >= 0 )
			YGNodeStyleSetFlexShrink( node, n );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setMaxSize( Size n ) {
	if( node ) {
		if( n.width && type != ElementType::Layout )
			YGNodeStyleSetMaxWidth( node, n.width );
		else
			YGNodeStyleSetMaxWidth( node, YGUndefined );

		if( n.height && type != ElementType::Layout )
			YGNodeStyleSetMaxHeight( node, n.height );
		else
			YGNodeStyleSetMaxHeight( node, YGUndefined );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setMinSize( Size n ) {
	if( node ) {
		if( n.width && type != ElementType::Layout )
			YGNodeStyleSetMinWidth( node, n.width );
		else
			YGNodeStyleSetMinWidth( node, YGUndefined );

		if( n.height && type != ElementType::Layout )
			YGNodeStyleSetMinHeight( node, n.height );
		else
			YGNodeStyleSetMinHeight( node, YGUndefined );

		if( !calculating )
			dirty_layout = true;
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setSize( Size n ) {
	if( node ) {
		auto row = YGNodeStyleGetFlexDirection( node ) == YGFlexDirectionRow ||
				YGNodeStyleGetFlexDirection( node ) == YGFlexDirectionRowReverse;

		if( type != ElementType::Layout )
			if( row ) {
				if( n.width <= 0 )
					grow = old_grow;
				else {
					old_grow = grow;
					grow = 0;
				}
			}
			else {
				if( n.height <= 0 )
					grow = old_grow;
				else {
					old_grow = grow;
					grow = 0;
				}
			}

		if( n.width && type != ElementType::Layout )
			YGNodeStyleSetWidth( node, n.width );
		else
			YGNodeStyleSetWidth( node, YGUndefined );

		if( n.height && type != ElementType::Layout )
			YGNodeStyleSetHeight( node, n.height );
		else
			YGNodeStyleSetHeight( node, YGUndefined );
	}

	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setPosition( Point n ) {
	if( node ) {

		if( n.x && type != ElementType::Layout )
			YGNodeStyleSetPosition( node, YGEdgeLeft, n.x );
		else
			YGNodeStyleSetPosition( node, YGEdgeLeft, YGUndefined );

		if( n.y && type != ElementType::Layout )
			YGNodeStyleSetPosition( node, YGEdgeTop, n.y );
		else
			YGNodeStyleSetPosition( node, YGEdgeTop, YGUndefined );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setAlignment( Alignment n ) {
	if( node ) {
		auto row = YGNodeStyleGetFlexDirection( node ) == YGFlexDirectionRow ||
				YGNodeStyleGetFlexDirection( node ) == YGFlexDirectionRowReverse;
		if( type == ElementType::Layout ) {

			if( flags( n & Alignment::Center ) ) {
				YGNodeStyleSetAlignItems( node, YGAlignCenter );
				YGNodeStyleSetJustifyContent( node, YGJustifyCenter );
			}

			if( flags( n & Alignment::Default ) ) {
				YGNodeStyleSetAlignItems( node, YGAlignStretch );
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::Left ) ) {

				if( row )
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
				else
					YGNodeStyleSetAlignItems( node, YGAlignFlexStart );
			}

			if( flags( n & Alignment::Top ) ) {
				if( row )
					YGNodeStyleSetAlignItems( node, YGAlignFlexStart );
				else
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::Right ) ) {
				if( row )
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
				else
					YGNodeStyleSetAlignItems( node, YGAlignFlexEnd );
			}

			if( flags( n & Alignment::Bottom ) ) {

				if( row )
					YGNodeStyleSetAlignItems( node, YGAlignFlexEnd );
				else
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
			}

			if( flags( n & Alignment::Start ) ) {
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::End ) ) {
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
			}
		}
		else {

			if( flags( n & Alignment::Center ) ) {
				YGNodeStyleSetAlignSelf( node, YGAlignCenter );
				YGNodeStyleSetJustifyContent( node, YGJustifyCenter );
			}

			if( flags( n & Alignment::Default ) ) {
				YGNodeStyleSetAlignItems( node, YGAlignStretch );
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::Left ) ) {

				if( row )
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
				else
					YGNodeStyleSetAlignItems( node, YGAlignFlexStart );
			}

			if( flags( n & Alignment::Top ) ) {
				if( row )
					YGNodeStyleSetAlignItems( node, YGAlignFlexStart );
				else
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::Right ) ) {
				if( row )
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
				else
					YGNodeStyleSetAlignItems( node, YGAlignFlexEnd );
			}

			if( flags( n & Alignment::Bottom ) ) {

				if( row )
					YGNodeStyleSetAlignItems( node, YGAlignFlexEnd );
				else
					YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
			}

			if( flags( n & Alignment::Start ) ) {
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );
			}

			if( flags( n & Alignment::End ) ) {
				YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );
			}
		}
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setMargin( float n, Direction d ) {
	if( node ) {

		if( flags( d & Direction::Left ) ) {
			YGNodeStyleSetMargin( node, YGEdgeLeft, n );
		}

		if( flags( d & Direction::Top ) ) {
			YGNodeStyleSetMargin( node, YGEdgeTop, n );
		}

		if( flags( d & Direction::Right ) ) {
			YGNodeStyleSetMargin( node, YGEdgeRight, n );
		}

		if( flags( d & Direction::Bottom ) ) {
			YGNodeStyleSetMargin( node, YGEdgeBottom, n );
		}

	}
	if( !calculating )
		dirty_layout = true;
}

Layout* Layoutable::getLayout() const {
	return in_layout;
}

_LayoutConfig Layout::layout_config = nullptr;

Layout::Layout() : Layoutable(),
                   position( [](Point p)-> Point { return p; }, Layoutable::position ),
                   size( [](Size s)-> Size { return s; }, Layoutable::size ),
                   minSize( [](Size s)-> Size { return s; }, Layoutable::minSize ),
                   maxSize( [](Size s)-> Size { return s; }, Layoutable::maxSize ),
                   orientation( Orientation::Default ),
                   wrap( false ),
                   widget( this, std::mem_fn( &Layout::getWidget ), std::mem_fn( &Layout::setWidget ) ) {

	setType( ElementType::Layout );
	if (!layout_config) {
		layout_config = YGConfigNew();
		YGConfigSetUseWebDefaults(layout_config, true);
	}

	node = YGNodeNewWithConfig(layout_config);
	objectName = "Layout";

	orientation.changed( [&](Orientation n) {
		                    setOrientation( n );
	                    } );
	wrap.changed( [&](bool n) {
		             setWrap( n );
	             } );

	Layout::applyStyle();
}

Layout::~Layout() {
	if (layout_config)
		YGConfigFree(layout_config);
}

void Layout::append( Layoutable* item, Alignment align ) {
	if( !item )
		return;

	auto l = item->in_layout;

	if( l == this )
		return; // TODO: maybe warn?

	// take from old layout
	if( l )
		l->take( item );

	item->in_layout = this;

	if( !item->node ) {
		item->node = YGNodeNewWithConfig(layout_config);
	}

	auto n = YGNodeGetChildCount( node );
	YGNodeInsertChild( node, item->node, n );
	nodemap.insert( { item->node, item } );

	// take ownership of layout
	if( item->type == ElementType::Layout )
		item->parent = this;
	else
		item->parent = _widget; // or set managing widget as parent

	item->applyStyle();

	dirty_layout = true;
}

void Layout::append( std::initializer_list< priv::Layoutable* > item, Alignment align ) {
	for( auto& i : item ) {
		append( i, align );
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

		// remove parent
		item->parent = nullptr;

	}
}

void Layout::invalidate() {

	Rect r;

	if( _widget ) { // if this layout is managing a widget, use the widget's properties
		r = _widget->geometry;
		YGNodeCalculateLayout( node, r.width, r.height, YGDirectionLTR );
	}
	else { // else, use our own properties
		YGNodeCalculateLayout( node, YGUndefined, YGUndefined, YGDirectionLTR );
	}

	// apply new calculations
	invalidated();

	// inform all underlying layouts to recalculate
	for( auto i : nodemap ) {
		i.second->parent_has_calculated = true;
		if( i.second->type == ElementType::Layout )
			static_cast< Layout* >( i.second )->invalidate();
		else
			i.second->invalidated();
	}
}

void Layout::setWidget( Widget* w ) {
	if( w && w->bound_layout ) // this wigdet is already handled by a layout
		return; // TODO: Maybe inform user?

	if( w->bound_layout == this )
		return;

	_widget = w;
	for( auto i : nodemap ) {
		if( i.second->type == ElementType::Widget )
			i.second->parent = _widget;
	}

	if( w ) {
		w->bound_layout = this;
		/*if (w->node) {
			YGNodeCopyStyle(node, w->node);
			if (w->in_layout) {
				YGNodeRemoveChild(w->in_layout->node, w->node);
				w->in_layout->nodemap.erase(w->node);
				
				w->node = node;

				auto n = YGNodeGetChildCount(w->in_layout->node);
				YGNodeInsertChild(w->in_layout->node, w->node, n);
				w->in_layout->nodemap.insert({ w->node, w });
			}
		} else {
			w->node = node;
		}*/
	}
}

void Layout::setOrientation( Orientation n ) {
	if( node ) {

		if( flags( n & Orientation::Default ) || flags( n & Orientation::Horizontal ) ) {
			YGNodeStyleSetFlexDirection( node, YGFlexDirectionRow );
		}

		if( flags( n & Orientation::Vertical ) ) {
			YGNodeStyleSetFlexDirection( node, YGFlexDirectionColumn );
		}

		if( flags( n & Orientation::Reverse ) ) {
			switch( YGNodeStyleGetFlexDirection( node ) ) { // only need to cover these two
				case YGFlexDirectionRow:
					YGNodeStyleSetFlexDirection( node, YGFlexDirectionRowReverse );
					break;
				case YGFlexDirectionColumn:
					YGNodeStyleSetFlexDirection( node, YGFlexDirectionColumnReverse );
					break;
			}
		}
	}
	if( !calculating )
		dirty_layout = true;
}

void Layout::setWrap( bool n ) {
	if( node ) {
		YGNodeStyleSetFlexWrap( node, n ? YGWrapWrap : YGWrapNoWrap );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layout::invalidated() {
	Layoutable::invalidated();

	if( node && parent_has_calculated ) {}
}

void Layout::applyStyle() {
	Layoutable::applyStyle();

	setOrientation( orientation );
	setWrap( wrap );

}

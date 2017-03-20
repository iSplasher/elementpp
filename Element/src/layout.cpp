#include "element/layout.h"
#include "element/widget.h"
#include "deps/yoga/Yoga.h"

USING_NAMESPACE

_LayoutConfig Layoutable::layout_config = nullptr;


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
                                               justifyContent( Justify::Default ),
                                               alignItems( Align::Default ),
                                               alignSelf( Align::Default ),
                                               grow( 0 ),
                                               shrink( 0 ),
                                               basis( 0 ),
                                               positionType( Position::Default ),
                                               direction( Direction::Default ),
                                               wrap( false ) {

	if( !layout_config ) {
		layout_config = YGConfigNew();
		YGConfigSetUseWebDefaults( layout_config, true );
	}

	node = YGNodeNewWithConfig( layout_config );

	// connect properties
	position.changed( [&](Point n) {
		                 setPosition( n );
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
		                   }
	                   } );
	marginTop.changed( [&](float n) {
		                  if( node ) {
			                  setMargin( n, Direction::Top );
		                  }
	                  } );
	marginRight.changed( [&](float n) {
		                    if( node ) {
			                    setMargin( n, Direction::Right );
		                    }
	                    } );
	marginBottom.changed( [&](float n) {
		                     if( node ) {
			                     setMargin( n, Direction::Bottom );
		                     }
	                     } );

	borderLeft.changed( [&](float n) {
		                   if( node ) {
			                   setBorder( n, Direction::Left );
		                   }
	                   } );
	borderTop.changed( [&](float n) {
		                  if( node ) {
			                  setBorder( n, Direction::Top );
		                  }
	                  } );
	borderRight.changed( [&](float n) {
		                    if( node ) {
			                    setBorder( n, Direction::Right );
		                    }
	                    } );
	borderBottom.changed( [&](float n) {
		                     if( node ) {
			                     setBorder( n, Direction::Bottom );
		                     }
	                     } );

	paddingLeft.changed( [&](float n) {
		                    if( node ) {
			                    setPadding( n, Direction::Left );
		                    }
	                    } );
	paddingTop.changed( [&](float n) {
		                   if( node ) {
			                   setPadding( n, Direction::Top );
		                   }
	                   } );
	paddingRight.changed( [&](float n) {
		                     if( node ) {
			                     setPadding( n, Direction::Right );
		                     }
	                     } );
	paddingBottom.changed( [&](float n) {
		                      if( node ) {
			                      setPadding( n, Direction::Bottom );
		                      }
	                      } );

	grow.changed( [&](float n) {
		             setGrow( n );
	             } );
	shrink.changed( [&](float n) { setShrink( n ); } );
	basis.changed( [&](float n) { setBasis( n ); } );

	positionType.changed( [&](Position n) {
		                     if( node ) {
			                     if( flags( Position::Absolute & n ) )
				                     YGNodeStyleSetPositionType( node, YGPositionTypeAbsolute );

			                     if( flags( Position::Relative & n ) )
				                     YGNodeStyleSetPositionType( node, YGPositionTypeRelative );

			                     if( !calculating )
				                     dirty_layout = true;
		                     }
	                     } );

	justifyContent.changed( [&](Justify n) {
		                       if( node ) {
			                       if( flags( Justify::Start & n ) )
				                       YGNodeStyleSetJustifyContent( node, YGJustifyFlexStart );

			                       if( flags( Justify::End & n ) )
				                       YGNodeStyleSetJustifyContent( node, YGJustifyFlexEnd );

			                       if( flags( Justify::Center & n ) )
				                       YGNodeStyleSetJustifyContent( node, YGJustifyCenter );

			                       if( flags( Justify::SpaceAround & n ) )
				                       YGNodeStyleSetJustifyContent( node, YGJustifySpaceAround );

			                       if( flags( Justify::SpaceBetween & n ) )
				                       YGNodeStyleSetJustifyContent( node, YGJustifySpaceBetween );

			                       if( !calculating )
				                       dirty_layout = true;
		                       }
	                       } );

	alignItems.changed( [&](Align n) {
		                   if( node ) {
			                   if( flags( Align::Start & n ) )
				                   YGNodeStyleSetAlignItems( node, YGAlignFlexStart );

			                   if( flags( Align::Stretch & n ) )
				                   YGNodeStyleSetAlignItems( node, YGAlignStretch );

			                   if( flags( Align::End & n ) )
				                   YGNodeStyleSetAlignItems( node, YGAlignFlexEnd );

			                   if( flags( Align::Center & n ) )
				                   YGNodeStyleSetAlignItems( node, YGAlignCenter );

			                   if( !calculating )
				                   dirty_layout = true;
		                   }
	                   } );

	alignSelf.changed( [&](Align n) {
		                  if( node ) {
			                  if( flags( Align::Start & n ) )
				                  YGNodeStyleSetAlignSelf( node, YGAlignFlexStart );

			                  if( flags( Align::Stretch & n ) )
				                  YGNodeStyleSetAlignSelf( node, YGAlignStretch );

			                  if( flags( Align::End & n ) )
				                  YGNodeStyleSetAlignSelf( node, YGAlignFlexEnd );

			                  if( flags( Align::Center & n ) )
				                  YGNodeStyleSetAlignSelf( node, YGAlignCenter );

			                  if( !calculating )
				                  dirty_layout = true;
		                  }
	                  } );

	direction.changed( [&](Direction n) {
		                  if( node ) {
			                  if( flags( Direction::Left & n ) )
				                  YGNodeStyleSetFlexDirection( node, YGFlexDirectionRowReverse );

			                  if( flags( Direction::Top & n ) )
				                  YGNodeStyleSetFlexDirection( node, YGFlexDirectionColumnReverse );

			                  if( flags( Direction::Right & n ) )
				                  YGNodeStyleSetFlexDirection( node, YGFlexDirectionRow );

			                  if( flags( Direction::Bottom & n ) )
				                  YGNodeStyleSetFlexDirection( node, YGFlexDirectionColumn );

			                  if( !calculating )
				                  dirty_layout = true;
		                  }
	                  } );

	// set default values

	grow = 1;
}

Layoutable::~Layoutable() {
	if( node )
		YGNodeFree( node );

	if( layout_config )
		YGConfigFree( layout_config );
}


void Layoutable::append( Layoutable* item ) {
	if( !item )
		return;

	auto l = static_cast< Layoutable* >( item->parent.get() );

	// take from old widget
	if( l && l != this )
		l->take( item );

	if( l != this ) {
		item->parent = this;
	}

	if( !item->node ) {
		item->node = YGNodeNewWithConfig( layout_config );
	}

	if( !nodemap.count( item->node ) ) {
		auto n = YGNodeGetChildCount( node );
		YGNodeInsertChild( node, item->node, n );
		nodemap.insert( { item->node, item } );
	}

	dirty_layout = true;
}

void Layoutable::append( std::initializer_list< Layoutable* > item ) {
	for( auto& i : item ) {
		append( i );
	}
}

void Layoutable::take( Layoutable* item ) {
	if( item->parent.get() == this ) {
		if( nodemap.count( item->node ) ) {
			YGNodeRemoveChild( node, item->node );
			nodemap.erase( item->node );
		}
		item->parent = nullptr;
	}
}

void Layoutable::invalidate() {
	auto is_window = type == ElementType::Window;
	if( parent.get() || is_window ) { // if this layout is managing a widget, use the widget's properties
		Size s;
		if( is_window ) {
			s = this->size;
		}
		else {
			s = static_cast< Layoutable* >( parent.get() )->size;
		}
		YGNodeCalculateLayout( node, s.width, s.height, YGDirectionLTR );
	}
	else { // else, use our own properties
		YGNodeCalculateLayout( node, YGUndefined, YGUndefined, YGDirectionLTR );
	}

	// apply new calculations
	invalidated();

	// inform all underlying items
	for( auto i : nodemap ) {
		i.second->calculating = true;
		i.second->invalidated();
		i.second->calculating = false;
	}

}

void Layoutable::update() {

	if( dirty_layout ) {
		calculating = true;
		invalidate();
		calculating = false;
		dirty_layout = false;
	}

	updateChildren();
}

bool Layoutable::inLayout( Layoutable* w ) const {
	if( w->node )
		return nodemap.count( w->node );
	return false;
}

void Layoutable::setPixelRatio( float p ) {
	if (layout_config) {
		YGConfigSetPointScaleFactor(layout_config, p);
	}
}

void Layoutable::updateChildren() {
	for( auto c : children() ) {
		static_cast< Layoutable* >( c )->update();
	}
}

void Layoutable::invalidated() {
	if( node ) {
		float nan1, nan2;
		if( type != ElementType::Window ) {

			position = Point( YGNodeLayoutGetLeft( node ), YGNodeLayoutGetTop( node ) );
			size = Size( YGNodeLayoutGetWidth( node ), YGNodeLayoutGetHeight( node ) );

			nan1 = YGNodeStyleGetMinWidth(node).value;
			nan2 = YGNodeStyleGetMinHeight( node ).value;
			minSize = Size(std::isnan( nan1 ) ? 0 : nan1, std::isnan( nan2 ) ? 0 : nan2);
			nan1 = YGNodeStyleGetMaxWidth(node).value;
			nan2 = YGNodeStyleGetMaxHeight(node).value;
			maxSize = Size(std::isnan(nan1) ? 0 : nan1, std::isnan(nan2) ? 0 : nan2);

			nan1 = YGNodeStyleGetMargin( node, YGEdgeLeft ).value;
			marginLeft = std::isnan(nan1) ? 0 : nan1;
			nan1 = YGNodeStyleGetMargin(node, YGEdgeTop).value;
			marginTop = std::isnan(nan1) ? 0 : nan1;
			nan1 = YGNodeStyleGetMargin(node, YGEdgeRight).value;
			marginRight = std::isnan(nan1) ? 0 : nan1;
			nan1 = YGNodeStyleGetMargin(node, YGEdgeBottom).value;
			marginBottom = std::isnan(nan1) ? 0 : nan1;

		}

		nan1 = YGNodeStyleGetBorder(node, YGEdgeLeft);
		borderLeft = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetBorder(node, YGEdgeTop);
		borderTop = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetBorder(node, YGEdgeRight);
		borderRight = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetBorder(node, YGEdgeBottom);
		borderBottom = std::isnan(nan1) ? 0 : nan1;

		nan1 = YGNodeStyleGetPadding(node, YGEdgeLeft).value;
		paddingLeft = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetPadding(node, YGEdgeTop).value;
		paddingTop = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetPadding(node, YGEdgeRight).value;
		paddingRight = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetPadding(node, YGEdgeBottom).value;
		paddingBottom = std::isnan(nan1) ? 0 : nan1;

		nan1 = YGNodeStyleGetFlexGrow(node);
		grow = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetFlexShrink(node);
		shrink = std::isnan(nan1) ? 0 : nan1;
		nan1 = YGNodeStyleGetFlexBasis(node).value;
		basis = std::isnan(nan1) ? 0 : nan1;
	}
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

void Layoutable::setBasis( float n ) {
	if( node ) {
		if( n >= 0 )
			YGNodeStyleSetFlexBasis( node, n );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setMaxSize( Size n ) {
	if( node ) {
		YGNodeStyleSetMaxWidth( node, n.width );
		YGNodeStyleSetMaxHeight( node, n.height );
	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setMinSize( Size n ) {
	if( node ) {
		YGNodeStyleSetMinWidth( node, n.width );
		YGNodeStyleSetMinHeight( node, n.height );

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

		//if( type != ElementType::Layout ) {
		//	if( row ) {
		//		if( n.width <= 0 )
		//			grow = old_grow;
		//		else {
		//			old_grow = grow;
		//			grow = 0;
		//		}
		//	}
		//	else {
		//		if( n.height <= 0 )
		//			grow = old_grow;
		//		else {
		//			old_grow = grow;
		//			grow = 0;
		//		}
		//	}
		//}

		YGNodeStyleSetWidth( node, n.width );
		YGNodeStyleSetHeight( node, n.height );
	}

	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setPosition( Point n ) {
	if( type != ElementType::Window ) {
		if( node ) {

			YGNodeStyleSetPosition( node, YGEdgeLeft, n.x );
			YGNodeStyleSetPosition( node, YGEdgeTop, n.y );
		}
		if( !calculating )
			dirty_layout = true;
	}
}

void Layoutable::setMargin( float n, Direction d ) {
	if( type != ElementType::Window ) {
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
}

void Layoutable::setBorder( float n, Direction d ) {
	if( node ) {

		if( flags( d & Direction::Left ) ) {
			YGNodeStyleSetBorder( node, YGEdgeLeft, n );
		}

		if( flags( d & Direction::Top ) ) {
			YGNodeStyleSetBorder( node, YGEdgeTop, n );
		}

		if( flags( d & Direction::Right ) ) {
			YGNodeStyleSetBorder( node, YGEdgeRight, n );
		}

		if( flags( d & Direction::Bottom ) ) {
			YGNodeStyleSetBorder( node, YGEdgeBottom, n );
		}

	}
	if( !calculating )
		dirty_layout = true;
}

void Layoutable::setPadding( float n, Direction d ) {
	if( node ) {

		if( flags( d & Direction::Left ) ) {
			YGNodeStyleSetPadding( node, YGEdgeLeft, n );
		}

		if( flags( d & Direction::Top ) ) {
			YGNodeStyleSetPadding( node, YGEdgeTop, n );
		}

		if( flags( d & Direction::Right ) ) {
			YGNodeStyleSetPadding( node, YGEdgeRight, n );
		}

		if( flags( d & Direction::Bottom ) ) {
			YGNodeStyleSetPadding( node, YGEdgeBottom, n );
		}

	}
	if( !calculating )
		dirty_layout = true;
}

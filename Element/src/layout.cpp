#include "element/layout.h"
#include "element/widget.h"

#include <yoga/Yoga.h>


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

	// set default values
	size = SizeF( 200, 200 );

	// connect properties

}

Layoutable::~Layoutable() {}


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
		position = PointF( YGNodeLayoutGetLeft( node ), YGNodeLayoutGetTop( node ) );
		size = SizeF( YGNodeLayoutGetWidth( node ), YGNodeLayoutGetHeight( node ) );
	}
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

	applySet( item, align, grow );

	dirty_layout = true;
}

void Layout::append( std::initializer_list< priv::Layoutable* > item, Alignment align, float grow ) {
	for( auto& i : item ) {
		append( i, align, grow );
	}
}

void Layout::update() {
	if( dirty_layout )
		invalidate();
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

	YGNodeCalculateLayout( node, r.width, r.height, YGDirectionInherit );

	// apply new calculations
	invalidated();

	// inform all underlying layouts to recalculate
	for( auto i : nodemap ) {
		if (i.second->type == ElementType::Layout)
			static_cast<Layout*>(i.second)->invalidate();
		else
			i.second->invalidated();
	}
}

void Layout::invalidated() {
	Layoutable::invalidated();

	if (node) {
	}
}

void Layout::applySet( Layoutable* item, Alignment align, float grow ) {

	//if( grow > 0 ) {
	//	prop.grow = grow;
	//	prop.shrink = 0;
	//}
	//else if( grow < 0 ) {
	//	prop.grow = 0;
	//	prop.shrink = grow;
	//}
	//YGNodeStyleSetFlexGrow( item->node,  );
	//YGNodeStyleSetFlexShrink( item->node, prop.shrink );

	//YGNodeStyleSetMargin( item->node, YGEdgeLeft, prop.margin_left );
	//YGNodeStyleSetMargin( item->node, YGEdgeTop, prop.margin_top );
	//YGNodeStyleSetMargin( item->node, YGEdgeRight, prop.margin_right );
	//YGNodeStyleSetMargin( item->node, YGEdgeBottom, prop.margin_bottom );

	//YGNodeStyleSetBorder( item->node, YGEdgeLeft, prop.border_left );
	//YGNodeStyleSetBorder( item->node, YGEdgeTop, prop.border_top );
	//YGNodeStyleSetBorder( item->node, YGEdgeRight, prop.border_right );
	//YGNodeStyleSetBorder( item->node, YGEdgeBottom, prop.border_bottom );

	//YGNodeStyleSetPadding( item->node, YGEdgeLeft, prop.padding_left );
	//YGNodeStyleSetPadding( item->node, YGEdgeTop, prop.padding_top );
	//YGNodeStyleSetPadding( item->node, YGEdgeRight, prop.padding_right );
	//YGNodeStyleSetPadding( item->node, YGEdgeBottom, prop.padding_bottom );
}

void Layout::applyGet( priv::Layoutable* item ) { }

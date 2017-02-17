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

LayoutElement::LayoutElement( LayoutElement* parent ) : Element( parent ),
                                                           geometry( [](Point pos, Size size) -> Rect {
	                                                                     return Rect( pos, size );
                                                                     }, position, size )
                                                            {
	
	if( parent && parent->type == ElementType::Layout ) {
		auto l = dynamic_cast<Layout*>(parent);
		l->appendItem( this );
	}
}

LayoutElement::~LayoutElement() {}


void LayoutElement::update() {
	updateChildren();
}

//void LayoutCore::resizeEvent( ResizeEventPtr ev ) {
//	properties.size.width = ev->size.width;
//	properties.size.height = ev->size.height;
//	if( node ) {
//		YGNodeStyleSetWidth( node, properties.size.width );
//		YGNodeStyleSetHeight( node, properties.size.height );
//	}
//	if( playout ) {
//		playout->invalidate();
//	}
//
//}

void LayoutElement::updateChildren() {
	for( auto c : children() ) {
		static_cast< LayoutElement* >( c )->update();
	}
}

void LayoutElement::updateGeometry() {
	if( dirty_layuot ) {
		if( node ) {
			auto& p = properties;
			p.position.x = YGNodeLayoutGetLeft( node );
			p.position.y = YGNodeLayoutGetTop( node );
			p.size.width = YGNodeLayoutGetWidth( node );
			p.size.height = YGNodeLayoutGetHeight( node );
		}
		dirty_layuot = false;
	}
}

Layout::Layout() : LayoutElement() {
	setType(ElementType::Layout);
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

void Layout::appendItem(LayoutElement* item, Alignment align, float grow ) {
	auto l = item->layout;

	if( l == this )
		return; // TODO: maybe warn?

	if( l )
		l->takeItem( item );

	if( !item->node ) {
		item->node = YGNodeNew();
	}

	auto n = YGNodeGetChildCount( node );
	YGNodeInsertChild( node, item->node, n );
	nodemap.insert( { item->node, item } );

	applyItemProperties( item, align, grow );

	invalidate();
}

void Layout::takeItem( LayoutElement* item ) {
	if( item->layout.get() == this ) {
		YGNodeRemoveChild( node, item->node );
		nodemap.erase( item->node );
		item->playout = nullptr;
	}
}

void Layout::invalidate() {

	if( _widget ) {
		auto& pr = properties;
		Rect r = _widget->geometry;
		pr.size.width = r.width;
		pr.size.height = r.height;
		YGNodeCalculateLayout( node, pr.size.width, pr.size.height, YGDirectionInherit );
	}
	else {

		auto& pr = properties;

		YGNodeCalculateLayout( node, pr.size.width, pr.size.height, YGDirectionInherit );
	}

	for( auto i : nodemap ) {
		i.second->dirty_layuot = true;
	}
}

void Layout::applyItemProperties( LayoutElement* item, Alignment align, float grow ) {
	auto prop = item->properties;
	if( grow > 0 ) {
		prop.grow = grow;
		prop.shrink = 0;
	}
	else if( grow < 0 ) {
		prop.grow = 0;
		prop.shrink = grow;
	}
	YGNodeStyleSetFlexGrow( item->node, prop.grow );
	YGNodeStyleSetFlexShrink( item->node, prop.shrink );

	YGNodeStyleSetMargin( item->node, YGEdgeLeft, prop.margin_left );
	YGNodeStyleSetMargin( item->node, YGEdgeTop, prop.margin_top );
	YGNodeStyleSetMargin( item->node, YGEdgeRight, prop.margin_right );
	YGNodeStyleSetMargin( item->node, YGEdgeBottom, prop.margin_bottom );

	YGNodeStyleSetBorder( item->node, YGEdgeLeft, prop.border_left );
	YGNodeStyleSetBorder( item->node, YGEdgeTop, prop.border_top );
	YGNodeStyleSetBorder( item->node, YGEdgeRight, prop.border_right );
	YGNodeStyleSetBorder( item->node, YGEdgeBottom, prop.border_bottom );

	YGNodeStyleSetPadding( item->node, YGEdgeLeft, prop.padding_left );
	YGNodeStyleSetPadding( item->node, YGEdgeTop, prop.padding_top );
	YGNodeStyleSetPadding( item->node, YGEdgeRight, prop.padding_right );
	YGNodeStyleSetPadding( item->node, YGEdgeBottom, prop.padding_bottom );
}


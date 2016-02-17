#include "LayoutImpl.h"

USING_NAMESPACE

gLayoutImpl::gLayoutImpl(gLayout* p_layout) {
	layout = p_layout;

	simplex = std::make_unique<Simplex>();

	auto s = p_layout->size();
	auto p = p_layout->pos();
	layout_constraints.c_x = p.x;
	layout_constraints.c_y = p.y;
	layout_constraints.c_width = s.width;
	layout_constraints.c_height = s.height;

	// for x
	simplex->add_stay(layout_constraints.c_x, REQUIRED);
	simplex->add_stay(layout_constraints.c_width, WEAK); // inital
	// cannot be negative | set this to start x
	simplex->add_constraint(layout_constraints.c_x >= 0, REQUIRED);

	// for y
	simplex->add_stay(layout_constraints.c_y, REQUIRED);
	simplex->add_stay(layout_constraints.c_height, WEAK); // inital
	// cannot be negative
	simplex->add_constraint(layout_constraints.c_y >= 0, REQUIRED);
}

gLayoutImpl::LayoutItem gLayoutImpl::addItem(gLayoutable* item) {
	LayoutItem item_pair = std::make_pair(item, std::make_shared<ItemConstraints>());
	layout_items.push_back(item_pair);
	return item_pair;
}

ItemConstraintPtr gLayoutImpl::prevConstraint() const {
	ItemConstraintPtr prev = nullptr;
	if (!layout_items.empty() && layout_items.size() > 1) {
		prev = ((layout_items.end() - 2)->second);
	}
	return prev;
}

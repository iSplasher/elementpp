#include "LayoutImpl.h"

USING_NAMESPACE
using namespace priv;

LayoutImpl::LayoutImpl(gLayout* p_layout) {
	layout = p_layout;

	simplex = std::make_unique<Simplex>();

	// layout cannot go out of bounds
	simplex->add_constraint(p_layout->c_data->x >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->y >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->width >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->height >= 0, REQUIRED);
}

void LayoutImpl::addItem(gLayoutable* item) {
	// item cannot go out of bounds
	addConstraint(item->c_data->x >= 0, REQUIRED);
	addConstraint(item->c_data->y >= 0, REQUIRED);
	addConstraint(item->c_data->width >= 0, REQUIRED);
	addConstraint(item->c_data->height >= 0, REQUIRED);
	layout_items.push_back(item);
}

void LayoutImpl::addConstraint(rhea::constraint constraint) {
	simplex->add_constraint(constraint);
	constraints.push_back(constraint);
}

void LayoutImpl::addConstraint(rhea::linear_inequality exp, rhea::strength strength, double weigth) {
	addConstraint(rhea::constraint(exp, strength, weigth));
}

void LayoutImpl::addConstraint(rhea::linear_equation exp, rhea::strength strength, double weigth) {
	addConstraint(rhea::constraint(exp, strength, weigth));
	
}

void LayoutImpl::resetConstraints() {
	for (auto &c : constraints) {
		simplex->remove_constraint(c);
	}
	constraints.clear();
}

ItemData LayoutImpl::getData(gLayoutable* item) {
	return item ? item->c_data : nullptr;
}

ItemData LayoutImpl::prevConstraint() {
	ItemData item_data = nullptr;
	if (!layout_items.empty() && layout_items.size() > 1) {
		auto item_iter = layout_items.end()-2;
		item_data = (*item_iter)->c_data;
	}

	return item_data;
}

#include "LayoutImpl.h"
#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"

USING_NAMESPACE
using namespace priv;

LayoutImpl::LayoutImpl(gLayout* p_layout) {
	layout = p_layout;

	simplex = std::make_unique<Simplex>();
	simplex->set_autosolve(false);

	// layout cannot go out of bounds
	simplex->add_constraint(p_layout->c_data->x >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->y >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->width >= 0, REQUIRED);
	simplex->add_constraint(p_layout->c_data->height >= 0, REQUIRED);
	simplex->solve();
}

void LayoutImpl::addItem(gLayoutable* item) {

	auto &c_data = item->c_data;

	// item cannot go out of bounds
	simplex->add_constraint(c_data->x >= 0, REQUIRED);
	simplex->add_constraint(c_data->y >= 0, REQUIRED);
	simplex->add_constraint(c_data->width >= 0, REQUIRED);
	simplex->add_constraint(c_data->height >= 0, REQUIRED);

	// minimum & maximum constraints
	simplex->add_constraint(c_data->width >= c_data->minWidth, REQUIRED);
	simplex->add_constraint(c_data->width <= c_data->maxWidth, REQUIRED);
	simplex->add_constraint(c_data->height >= c_data->minHeight, REQUIRED);
	simplex->add_constraint(c_data->height <= c_data->maxHeight, REQUIRED);
	simplex->solve();

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

void LayoutImpl::setWidget(gCoreWidget* new_widget) {
	for (auto &c : parent_constraints) {
		simplex->remove_constraint(c);
	}
	constraints.clear();

	simplex->add_stay(new_widget->c_data->width, STRONG);
	simplex->add_stay(new_widget->c_data->height, STRONG);

	// TODO: consider widget margin here!!
	rhea::constraint x_c{ layout->c_data->x == 0, REQUIRED };
	simplex->add_constraint(x_c);
	parent_constraints.push_back(x_c);
	rhea::constraint y_c{ layout->c_data->y == 0, REQUIRED };
	simplex->add_constraint(y_c);
	parent_constraints.push_back(y_c);
	rhea::constraint width_c{ layout->c_data->width == new_widget->c_data->width, REQUIRED };
	simplex->add_constraint(width_c);
	parent_constraints.push_back(width_c);
	rhea::constraint height_c;
	if (new_widget->is_window) {
		auto n_window = static_cast<gWindow*>(new_widget);
		simplex->add_stay(n_window->top_bar->c_data->height, REQUIRED);
		height_c = { layout->c_data->height == n_window->c_data->height - n_window->top_bar->c_data->height, REQUIRED };
		rhea::constraint topbar_width_c = { n_window->c_data->width == n_window->top_bar->c_data->width, REQUIRED };
		simplex->add_constraint(topbar_width_c);
		parent_constraints.push_back(topbar_width_c);
	} else {
		height_c = layout->c_data->height == new_widget->c_data->height, REQUIRED;
	}
	simplex->add_constraint(height_c);
	parent_constraints.push_back(height_c);
	simplex->solve();

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

#include "gSplasher/Layouts/BoxLayout.h"
#include "../LayoutImpl.h"

USING_NAMESPACE

template<>
void gBoxLayout<Orientation::Vertical>::add(gLayoutable *item) {
	
}

template<>
void gBoxLayout<Orientation::Horizontal>::add(gLayoutable *item, Alignment align) {
	gLayout::add(item, align);
	auto item_pair = l_impl->addItem(item);
	auto &solver = l_impl->solver();
	
	auto item_size = item->size();
	auto c = item_pair.second;
	auto l_c = l_impl->constraints();

	auto space = 2;
	int left_margin, top_margin, right_margin, bottom_margin;
	left_margin = top_margin = right_margin = bottom_margin = margin();

	// for x
	solver->add_constraint(c->c_x >= l_c->c_x + 1, REQUIRED);
	solver->add_constraint(c->c_width >= l_c->minWidth, REQUIRED);
	solver->add_constraint(c->c_width <= l_c->maxWidth, REQUIRED);
	c->c_width = item_size.width;
	solver->add_stay(c->c_width, MEDIUM);

	// for y
	solver->add_constraint(c->c_y >= l_c->c_y + 1, REQUIRED);
	solver->add_constraint(c->c_height >= l_c->minHeight, REQUIRED);
	solver->add_constraint(c->c_height <= l_c->maxHeight, REQUIRED);
	c->c_height = item_size.height;
	solver->add_stay(c->c_height, MEDIUM);

	// left margin
	auto prev_constraint = l_impl->prevConstraint();
	if (prev_constraint) {
		solver->add_constraint(c->c_x >= prev_constraint->c_x + prev_constraint->c_width+1, REQUIRED);
		solver->add_constraint(c->c_x >= prev_constraint->c_x + prev_constraint->c_width + space, MEDIUM);
	} else {
		solver->add_constraint(c->c_x >= l_c->c_x + left_margin, REQUIRED);
	}

	// top margin
	solver->add_constraint(c->c_y == l_c->c_y + top_margin, REQUIRED);

	// right margin
	if (prev_constraint) {
		auto prev_c_width = prev_constraint->c_x + prev_constraint->c_width;
		for (auto &p_c : prev_constraint->right_constraints) {
			solver->remove_constraint(*p_c);
		}
		//solver->add_constraint(prev_c_width <= c->c_x + c->c_width - 1, REQUIRED);
		solver->add_constraint(prev_c_width <= c->c_x + c->c_width - space, STRONG);
	}
	ContraintPtr right_c = std::make_shared<rhea::constraint>(c->c_x + c->c_width <= l_c->c_x + l_c->c_width - right_margin, REQUIRED);
	c->right_constraints.push_back(right_c);
	solver->add_constraint(*right_c);

	// bottom margin
	solver->add_constraint(c->c_y + c->c_height == l_c->c_y + l_c->c_height - bottom_margin, REQUIRED);
	invalidate();
}

template<>
void gBoxLayout<Orientation::Vertical>::invalidate() {
	
}

template<>
void gBoxLayout<Orientation::Horizontal>::invalidate() {
	for (auto &item_pair : l_impl->items()) {
		auto &item = item_pair.first;
		auto &c = *item_pair.second;

		item->resize(c.c_width.int_value(), c.c_height.int_value());
		auto p = gPoint(c.c_x.int_value(), c.c_y.int_value());
		std::cout << p << std::endl;
		item->move(p);
	}
}
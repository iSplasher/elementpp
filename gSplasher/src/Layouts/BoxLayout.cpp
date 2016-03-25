#include "gSplasher/Layouts/BoxLayout.h"
#include "../LayoutImpl.h"

USING_NAMESPACE

template<>
void gBoxLayout<Orientation::Vertical>::add(gLayoutable *item, Alignment align) {

}

template<>
void gBoxLayout<Orientation::Horizontal>::add(gLayoutable *item, Alignment align) {
	gLayout::add(item, align);
}

template<>
void gBoxLayout<Orientation::Vertical>::invalidate() {

}

template<>
void gBoxLayout<Orientation::Horizontal>::invalidate() {
	if (!gApp->isRunning()) {
		return;
	}
	layouter->resetConstraints();

	gLayoutable* prev_item = nullptr;
	gLayoutable* next_item = nullptr;
	gLayoutable* item = nullptr;
	auto &items = layouter->items();
	auto space = gLayout::spacing();
	auto &solver = layouter->solver();

	// save old size for resizeEvent
	//for ()

	for (auto it = items.begin(); it != items.end(); ++it) {
		item = *it;
		next_item = nullptr;
		if (it + 1 != items.end()) {
			next_item = *(it + 1);
		}

		auto current_data = layouter->getData(item);
		auto prev_data = layouter->getData(prev_item);
		auto next_data = layouter->getData(next_item);
		auto this_data = layouter->getData(this);

		// make sure item can't be out of bounds
		if (prev_item) {
			layouter->addConstraint(current_data->x >= prev_data->x + prev_data->width + space, REQUIRED);
		}
		else {
			layouter->addConstraint(current_data->x >= this_data->x + space, REQUIRED);
		}

		if (next_item) {
			layouter->addConstraint(current_data->x + current_data->width <= next_data->x, REQUIRED);
		}
		else {
			layouter->addConstraint(current_data->x + current_data->width <= this_data->x + this_data->width - space, REQUIRED);
		}

		layouter->addConstraint(current_data->y >= this_data->y + space, REQUIRED);
		layouter->addConstraint(current_data->y + current_data->height <= this_data->y + this_data->height - space, REQUIRED);


		// the size can accommodate if not fixed
		if (!current_data->fixed_w_constraint) {
			solver->add_stay(current_data->width, WEAK);
		} else {
			layouter->addConstraint(current_data->width == current_data->fixed_width, STRONG, 2);
		}
		if (!current_data->fixed_h_constraint) {
			solver->add_stay(current_data->height, WEAK);
		} else {
			layouter->addConstraint(current_data->height == current_data->fixed_height, STRONG, 2);
		}

		// if width is not fixed then width should be the same
		if (next_item && !next_data->fixed_w_constraint) {
			layouter->addConstraint(current_data->width == next_data->width, MEDIUM, 2);
		} else {
			layouter->addConstraint(current_data->x+current_data->width == this_data->x+this_data->width-space, MEDIUM, 2);
		}

		// make it expanding in both directions
		layouter->addConstraint(current_data->x == this_data->x+space, MEDIUM);
		layouter->addConstraint(current_data->x+current_data->width == this_data->x+this_data->width-space, MEDIUM);
		layouter->addConstraint(current_data->y == this_data->y+space, MEDIUM);
		layouter->addConstraint(current_data->y+current_data->height == this_data->y+this_data->height-space, MEDIUM);

		// same with height
		if (!current_data->fixed_h_constraint) {
			layouter->addConstraint(current_data->height == this_data->height - (2 * space), MEDIUM);
		}

		prev_item = *it;
	}

	solver->solve();

}
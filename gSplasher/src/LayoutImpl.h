#pragma once

#include "gSplasher/Global.h"
#include "gSplasher/Layout.h"

#include <rhea/simplex_solver.hpp>
#include <vector>

NAMESPACE_BEGIN
using Simplex = rhea::simplex_solver;
#define REQUIRED rhea::strength::required()
#define STRONG rhea::strength::strong()
#define MEDIUM rhea::strength::medium()
#define WEAK rhea::strength::weak()

using ContraintPtr = std::shared_ptr<rhea::constraint>;
struct ItemConstraints {
	rhea::variable c_x;
	rhea::variable c_y;
	rhea::variable c_width;
	rhea::variable c_height;
	int minWidth = 10;
	int maxWidth = INT_MAX;
	int minHeight = 10;
	int maxHeight = INT_MAX;
	std::vector<ContraintPtr> right_constraints = {};

};

using ItemConstraintPtr = std::shared_ptr<ItemConstraints>;

class gLayoutImpl {
public:
	using LayoutItem = std::pair<gLayoutable*, ItemConstraintPtr>;
	using LayoutItems = std::vector<LayoutItem>;
	using Solver = std::unique_ptr<Simplex>;
	gLayoutImpl(gLayout *p_layout);

	LayoutItem addItem(gLayoutable* item);

	LayoutItems& items() { return layout_items; }
	Solver& solver() { return simplex; }
	ItemConstraints* constraints() { return &layout_constraints; }

	/// <summary>
	/// Returns the constraint item previous to the last added item
	/// </summary>
	/// <returns>Previous constraint item</returns>
	ItemConstraintPtr prevConstraint() const;

	LayoutItem* firstItem() { return layout_items.empty() ? nullptr : &layout_items.front(); }
	LayoutItem* lastItem() { return layout_items.empty() ? nullptr : &layout_items.back(); }

private:
	gLayout *layout = nullptr;
	Solver simplex;
	LayoutItems layout_items;

	ItemConstraints layout_constraints;
};

NAMESPACE_END
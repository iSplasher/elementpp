#pragma once

#include "gSplasher/Global.h"
#include "gSplasher/Layout.h"

#include <rhea/simplex_solver.hpp>
#include <vector>

NAMESPACE_BEGIN
namespace priv
{

	using Simplex = rhea::simplex_solver;
#define REQUIRED rhea::strength::required()
#define STRONG rhea::strength::strong()
#define MEDIUM rhea::strength::medium()
#define WEAK rhea::strength::weak()

	using ContraintPtr = std::shared_ptr<rhea::constraint>;
	struct ItemConstraints {
		int minWidth = 1;
		int maxWidth = INT_MAX;
		int minHeight = 1;
		int maxHeight = INT_MAX;
		rhea::variable x = 0;
		rhea::variable y = 0;
		rhea::variable width = minWidth;
		rhea::variable height = minHeight;

		rhea::constraint fixed_width_constraint;
		rhea::constraint fixed_height_constraint;
		bool fixed_w_constraint = false;
		bool fixed_h_constraint = false;
		int fixed_width = 0;
		int fixed_height = 0;

		gSize old_size;
	};

	class LayoutImpl {
	public:
		using LayoutItems = std::vector<gLayoutable*>;
		using Solver = std::unique_ptr<Simplex>;
		LayoutImpl(gLayout *p_layout);

		void addItem(gLayoutable* item);

		LayoutItems& items() { return layout_items; }
		int size() const { return layout_items.size(); }
		Solver& solver() { return simplex; }

		void addConstraint(rhea::constraint constraint);
		void addConstraint(rhea::linear_inequality exp, rhea::strength strength = REQUIRED, double weigth=1);
		void addConstraint(rhea::linear_equation exp, rhea::strength strength = REQUIRED, double weigth=1);
		void resetConstraints();

		void setWidget(gCoreWidget* new_widget);

		static ItemData getData(gLayoutable* item);

		/// <summary>
		/// Returns the constraint item previous to the last added item
		/// </summary>
		/// <returns>Previous constraint item</returns>
		ItemData prevConstraint();

		gLayoutable* firstItem() { return layout_items.empty() ? nullptr : layout_items.front(); }
		gLayoutable* lastItem() { return layout_items.empty() ? nullptr : layout_items.back(); }

	private:
		gLayout *layout = nullptr;
		Solver simplex;
		LayoutItems layout_items;
		rhea::constraint_list constraints;
		rhea::constraint_list parent_constraints;
	};
}
NAMESPACE_END
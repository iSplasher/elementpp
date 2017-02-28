#include "element/widget.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Widget objects can be created and destroyed", "[Widget]") {
	Application* app = Application::instance();
	if (!app)
		app = new Application();

	GIVEN("Widget objects are created") {
		auto comp1 = app->create< Widget >();
		auto comp2 = app->create< Widget >();

		REQUIRE(comp1);
		REQUIRE(comp2);


		WHEN("Parent of orphan Widget is called") {

			THEN("it is the nullparent") {
				REQUIRE(!comp1->parent);
			}
		}

		WHEN("Children are created") {
			auto child1 = app->create< Widget >(comp1);
			auto child2 = app->create< Widget >(comp2);

			THEN("they get appended to their parents") {
				REQUIRE(comp1->children().size() == 1);
				REQUIRE(comp2->children().size() == 1);

				REQUIRE(child1->parent == comp1);
				REQUIRE(child2->parent == comp2);
			}

			WHEN("More children are created") {
				auto child3 = app->create< Widget >(child1);
				auto child4 = app->create< Widget >(child2);

				THEN("they get appended to their parents") {
					REQUIRE(child1->children().size() == 1);
					REQUIRE(child2->children().size() == 1);

					REQUIRE(child3->parent == child1);
					REQUIRE(child4->parent == child2);
				}
			}
		}
	}
}

SCENARIO("Widgets", "[Widget]") {


}


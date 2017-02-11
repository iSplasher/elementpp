#include "element/core.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Application object can be created", "[Application]") {
	Application* app = Application::instance();
	if( !app )
		app = new Application();

	GIVEN("An Application object is created") {
		REQUIRE(app);
	}
}

SCENARIO("Component objects can be created and destroyed", "[Component]") {
	Application* app = Application::instance();
	if( !app )
		app = new Application();

	GIVEN("Component objects are created") {
		auto& comp1 = app->create< Component >();
		auto& comp2 = app->create< Component >();

		REQUIRE(comp1);
		REQUIRE(comp2);


		WHEN("Parent of orphan Component is called") {
			
			THEN("it is the nullparent") {
				REQUIRE(!comp1->parent);
			}
		}

		WHEN("Children are created") {
			auto& child1 = app->create< Component >( comp1 );
			auto& child2 = app->create< Component >( comp2 );

			THEN("they get appended to their parents") {
				REQUIRE(comp1->children().size() == 1);
				REQUIRE(comp2->children().size() == 1);

				REQUIRE(child1->parent == comp1);
				REQUIRE(child2->parent == comp2);
			}

			WHEN("One get destroyed") {
				app->destroy( child1 );

				THEN("One parent has no children") {
					REQUIRE(comp1->children().size() == 0);
					REQUIRE(comp2->children().size() == 1);
				}

			}

			WHEN("Both get destroyed") {
				app->destroy( child1 );
				app->destroy( child2 );

				THEN("All parents has no children") {
					REQUIRE(comp1->children().size() == 0);
					REQUIRE(comp2->children().size() == 0);
				}

			}

			// TODO: check if _parent is valid

		}
	}
}

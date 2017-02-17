#include "element/layout.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Layouts", "[Layout]") {
	Application* app = Application::instance();
	if (!app)
		app = new Application();

	GIVEN("Layouts are instatiated") {
		auto layout1 = app->create<Layout>();
		auto layout2 = app->create<Layout>();
		auto layout3 = app->create<Layout>();
		auto layout4 = app->create<Layout>();

		WHEN("A layout is appended") {
			layout1->append(layout2);
			
			THEN("Layout has taken ownership") {
				REQUIRE(layout1->children().size() == 1);
				REQUIRE(layout2->children().size() == 0);
				REQUIRE(layout3->children().size() == 0);
				REQUIRE(layout4->children().size() == 0);
			}

		}

		WHEN("Multiple layouts appended") {
			layout1->append(layout2);
			layout1->append({layout3, layout4});

			THEN("Layout has taken ownership") {
				REQUIRE(layout1->children().size() == 3);
				REQUIRE(layout2->children().size() == 0);
				REQUIRE(layout3->children().size() == 0);
				REQUIRE(layout4->children().size() == 0);
			}

		}

	}


}

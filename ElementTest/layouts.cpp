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

		WHEN("Default layout values") {

			THEN("Layout default values") {
				REQUIRE(layout1->size == SizeF(200, 200));
				REQUIRE(layout1->position == PointF(0, 0));
				REQUIRE(layout1->geometry == RectF(PointF(0, 0), SizeF(200, 200)));
			}

		}

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

			THEN("Default layout size is 200x200") {
				REQUIRE(layout1->size == SizeF(200, 200));

				WHEN("Layout is managing other layouts") {
					REQUIRE(layout2->size == SizeF(200, 200));
					REQUIRE(layout3->size == SizeF(200, 200));
					REQUIRE(layout4->size == SizeF(200, 200));

					WHEN("Layout is updated") {
						layout1->update();

						THEN("Layout sizes has been recalculated") {
							REQUIRE(layout1->size == SizeF(200, 200));
							REQUIRE(layout2->size == SizeF(200, 200));
							REQUIRE(layout3->size == SizeF(200, 200));
							REQUIRE(layout4->size == SizeF(200, 200));
						}

					}

				}

			}

		}

		WHEN("Layout has been destroyed") {
			layout1->append(layout2);
			layout1->append({ layout3, layout4 });
			app->destroy(layout3);
			app->destroy(layout4);

			THEN("All layouts have been destroyed") {
				REQUIRE(layout1->children().size() == 1);
			}

		}

	}


}

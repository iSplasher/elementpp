#include "element/layout.h"
#include "element/window.h"

#include "catch.hpp"

USING_NAMESPACE

void printWidget(PRIV_NAMESPACE::Layoutable* e) {
	std::cout << e->objectName << ": \n\t"
	<< "Geometry: " << e->geometry << "\n\t"
	<< "Margin: (" << e->marginLeft << ", " << e->marginTop << ", " << e->marginRight << ", " << e->marginBottom << ")" << "\n\t"
	<< "Border: (" << e->borderLeft << ", " << e->borderTop << ", " << e->borderRight << ", " << e->borderBottom << ")" << "\n\t"
	<< "Padding: (" << e->paddingLeft << ", " << e->paddingTop << ", " << e->paddingRight << ", " << e->paddingBottom << ")" << "\n\t"
	<< std::endl;
}

SCENARIO("Layouts", "[Layout]") {
	Application* app = Application::instance();
	if( !app )
		app = new Application();

	//auto root = priv::YGNodeNew();
	//auto child1 = priv::YGNodeNew();
	//auto child2 = priv::YGNodeNew();
	//YGNodeStyleSetPadding( root, priv::YGEdgeAll, 10 );
	//priv::YGNodeInsertChild( root, child1, 0 );
	//priv::YGNodeInsertChild( root, child2, 1 );
	////priv::YGNodeStyleSetAlignItems( root, priv::YGAlignStretch );
	//priv::YGNodeStyleSetFlexDirection( root, priv::YGFlexDirectionRow );
	//priv::YGNodeStyleSetFlexGrow( root, 1 );
	//priv::YGNodeStyleSetFlexGrow( child1, 1 );
	//priv::YGNodeStyleSetFlexGrow( child2, 1 );
	////priv::YGNodeStyleSetWidth(root, 300);
	////priv::YGNodeStyleSetHeight(root, 300);
	//priv::YGNodeStyleSetWidth( child1, 50 );
	//priv::YGNodeStyleSetHeight( child1, 50 );
	//priv::YGNodeCalculateLayout( root, 300, 300, priv::YGDirectionLTR );

	//priv::YGNodeStyleGetHeight( root );
	//priv::YGNodeStyleGetWidth( root );
	//priv::YGNodeStyleGetHeight( child1 );
	//priv::YGNodeStyleGetWidth( child1 );
	//priv::YGNodeLayoutGetHeight( root );
	//priv::YGNodeLayoutGetWidth( root );
	//priv::YGNodeLayoutGetHeight( child1 );
	//priv::YGNodeLayoutGetWidth( child1 );
	//priv::YGNodeLayoutGetHeight( child2 );
	//priv::YGNodeLayoutGetWidth( child2 );
	//priv::YGNodeLayoutGetTop( child1 );
	//priv::YGNodeLayoutGetLeft( child1 );
	//priv::YGNodeLayoutGetTop( child2 );
	//priv::YGNodeLayoutGetLeft( child2 );

	auto window = app->create<Window>();
	auto layout = app->create< Layout >();
	layout->widget = window;
	auto widget1 = app->create< Widget >();
	widget1->size = SizeF(50, 50);
	widget1->alignment = Alignment::Center;
	auto widget2 = app->create< Widget >();
	widget2->size = SizeF(50, 0);
	layout->alignment;
	layout->append({ widget1 });
	layout->update();
	printWidget(window);
	printWidget(layout);
	printWidget(widget1);
	printWidget(widget2);

	app->exec();

	GIVEN("Layouts are instatiated") {
		auto widget = app->create< Widget >();
		widget->size = SizeF( 200, 200 );
		auto layout1 = app->create< Layout >();
		layout1->widget = widget;
		auto layout2 = app->create< Layout >();
		auto layout3 = app->create< Layout >();
		auto layout4 = app->create< Layout >();

		WHEN("Default layout values") {
			widget->update();

			THEN("Layout default values") {
				REQUIRE(layout1->size == SizeF(0, 0));
				REQUIRE(layout1->position == PointF(0, 0));
				REQUIRE(layout1->geometry == RectF(PointF(0, 0), SizeF(0, 0)));
			}

		}

		WHEN("A layout is appended") {
			layout1->append( layout2 );

			THEN("Layout has taken ownership") {
				REQUIRE(layout1->children().size() == 1);
				REQUIRE(layout2->children().size() == 0);
				REQUIRE(layout3->children().size() == 0);
				REQUIRE(layout4->children().size() == 0);
			}

		}

		WHEN("Multiple layouts appended") {
			layout1->append( layout2 );
			layout1->append( { layout3, layout4 } );

			THEN("Layout has taken ownership") {
				REQUIRE(layout1->children().size() == 3);
				REQUIRE(layout2->children().size() == 0);
				REQUIRE(layout3->children().size() == 0);
				REQUIRE(layout4->children().size() == 0);
			}

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

		WHEN("Layout has been destroyed") {
			layout1->append( layout2 );
			layout1->append( { layout3, layout4 } );
			app->destroy( layout3 );
			app->destroy( layout4 );

			THEN("All layouts have been destroyed") {
				REQUIRE(layout1->children().size() == 1);
			}

		}

	}

}

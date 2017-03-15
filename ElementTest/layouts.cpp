#include "element/layout.h"
#include "element/window.h"

#include "catch.hpp"

USING_NAMESPACE

void printWidget( PRIV_NAMESPACE::Layoutable* e ) {
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

	auto window = app->create< Window >();
	//window->resized.changed( [](Rect r) { std::cout << "Window: " << r << std::endl; } );
	//window->mouseMoved.changed( [&](MouseEvent m) { std::cout << "Window: " << window->position.get() << std::endl; } );
	//window->mouseMoved.changed( [](MouseEvent m) { std::cout << "Window: " << m.position << std::endl; } );
	//window->leftPress.changed( [](MouseEvent m) { std::cout << "Left Pressed: Window: " << m.position << std::endl; } );
	auto layout = app->create< Layout >();
	layout->widget = window;
	auto widget1 = app->create< Widget >(window);
	widget1->size = Size( 0, 0 );
	//widget1->alignment = Alignment::Center;
	//widget1->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 1: " << m.position << std::endl; } );
	//widget1->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 1: " << m.position << std::endl; } );
	//widget1->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 1: " << m.position << std::endl; } );
	//widget1->leftClick.changed( [](Point p) { std::cout << "Left Click: Widget 1: " << p << std::endl; } );
	//widget1->rightClick.changed( [](Point p) { std::cout << "Right Click: Widget 1: " << p << std::endl; } );
	//widget1->click.changed( [](MouseEvent m) { std::cout << "Clicked: Widget 1: " << m.position << std::endl; } );
	//widget1->leftDoublePress.changed([](Point p) { std::cout << "Double Left Click: Widget 1: " << p << std::endl; });
	//widget1->rightDoublePress.changed([](Point p) { std::cout << "Doubel Right Click: Widget 1: " << p << std::endl; });
	//widget1->doublePress.changed([](MouseEvent m) { std::cout << "Double Clicked: Widget 1: " << m.position << std::endl; });
	auto widget2 = app->create< Widget >(widget1);
	widget2->size = Size( 0, 0 );
	widget2->isResizeable = true;
	//widget2->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 2: " << m.position << std::endl; } );
	//widget2->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 2: " << m.position << std::endl; } );
	//widget2->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 2: " << m.position << std::endl; } );
	layout->alignment;
	layout->append( { widget1 } );
	layout->append( { widget2 } );
	layout->update();

	auto widget11 = app->create< Widget >( widget1 );
	//widget11->isResizeable = true;
	//widget11->resized.changed( [](Rect r) { std::cout << "Widget 3: " << r << std::endl; } );
	//widget11->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 3: " << m.position << std::endl; } );
	//widget11->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 3: " << m.position << std::endl; } );
	//widget11->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 3: " << m.position << std::endl; } );
	auto widget12 = app->create< Widget >( widget1 );
	//widget12->isResizeable = true;
	//widget12->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 4: " << m.position << std::endl; } );
	//widget12->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 4: " << m.position << std::endl; } );
	//widget12->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 4: " << m.position << std::endl; } );
	auto layout1 = app->create< Layout >();
	layout1->widget = widget1;
	layout1->append({widget11, widget12});

	//printWidget(window);
	//printWidget(layout);
	//printWidget(widget1);
	//printWidget(widget2);

	app->exec();

	GIVEN("Layouts are instatiated") {
		auto widget = app->create< Widget >();
		widget->size = Size( 200, 200 );
		auto layout1 = app->create< Layout >();
		layout1->widget = widget;
		auto layout2 = app->create< Layout >();
		auto layout3 = app->create< Layout >();
		auto layout4 = app->create< Layout >();

		WHEN("Default layout values") {
			widget->update();

			THEN("Layout default values") {
				REQUIRE(layout1->size == Size(0, 0));
				REQUIRE(layout1->position == Point(0, 0));
				REQUIRE(layout1->geometry == Rect(Point(0, 0), Size(0, 0)));
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
					REQUIRE(layout1->size == Size(200, 200));
					REQUIRE(layout2->size == Size(200, 200));
					REQUIRE(layout3->size == Size(200, 200));
					REQUIRE(layout4->size == Size(200, 200));
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

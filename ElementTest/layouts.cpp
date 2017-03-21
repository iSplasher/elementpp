#include "element/layout.h"
#include "element/window.h"

#include "catch.hpp"

USING_NAMESPACE

void printWidget( Layoutable* e ) {
	std::cout << e->objectName << ": \n\t"
			<< "Children: " << e->children().size() << "\n\t"
			<< "Geometry: " << e->geometry << "\n\t"
			<< "Margin: (" << e->marginLeft << ", " << e->marginTop << ", " << e->marginRight << ", " << e->marginBottom << ")" << "\n\t"
			<< "Border: (" << e->borderLeft << ", " << e->borderTop << ", " << e->borderRight << ", " << e->borderBottom << ")" << "\n\t"
			<< "Padding: (" << e->paddingLeft << ", " << e->paddingTop << ", " << e->paddingRight << ", " << e->paddingBottom << ")" << "\n\t"
			<< std::endl;
}

void setDefault( Widget* w ) {
	w->borderLeft = w->borderTop = w->borderRight = w->borderBottom = 5;
	w->paddingLeft = w->paddingTop = w->paddingRight = w->paddingBottom = 5;
	w->marginLeft = w->marginTop = w->marginRight = w->marginBottom = 5;
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
	window->position = Point(700, 450);
	window->size = Size(600, 400);
	//window->resized.changed( [](Rect r) { std::cout << "Window: " << r << std::endl; } );
	//window->mouseMoved.changed( [&](MouseEvent m) { std::cout << "Window: " << window->position.get() << std::endl; } );
	//window->mouseMoved.changed( [](MouseEvent m) { std::cout << "Window: " << m.position << std::endl; } );
	//window->leftPress.changed( [](MouseEvent m) { std::cout << "Left Pressed: Window: " << m.position << std::endl; } );
	//widget1->size = Size( 600, 400 );
	//widget1->alignment = Edge::Center;
	//widget1->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 1: " << m.position << std::endl; } );
	//widget1->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 1: " << m.position << std::endl; } );
	//widget1->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 1: " << m.position << std::endl; } );
	//widget1->leftClick.changed( [](Point p) { std::cout << "Left Click: Widget 1: " << p << std::endl; } );
	//widget1->rightClick.changed( [](Point p) { std::cout << "Right Click: Widget 1: " << p << std::endl; } );
	//widget1->click.changed( [](MouseEvent m) { std::cout << "Clicked: Widget 1: " << m.position << std::endl; } );
	//widget1->leftDoublePress.changed([](Point p) { std::cout << "Double Left Click: Widget 1: " << p << std::endl; });
	//widget1->rightDoublePress.changed([](Point p) { std::cout << "Doubel Right Click: Widget 1: " << p << std::endl; });
	//widget1->doublePress.changed([](MouseEvent m) { std::cout << "Double Clicked: Widget 1: " << m.position << std::endl; });
	auto widget1 = app->create< Widget >(window);
	setDefault(widget1);
	widget1->isResizeable = true;
	auto widget2 = app->create< Widget >();
	setDefault(widget2);
	widget2->objectName = "Widget2";
	//widget2->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 2: " << m.position << std::endl; } );
	//widget2->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 2: " << m.position << std::endl; } );
	//widget2->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 2: " << m.position << std::endl; } );
	//window->append( { widget1 } );
	//window->append( { widget2 } );

	auto widget11 = app->create< Widget >( widget1 );
	setDefault(widget11);
	widget11->grow = 2;
	//widget11->isResizeable = true;
	//widget11->resized.changed( [](Rect r) { std::cout << "Widget 3: " << r << std::endl; } );
	//widget11->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 3: " << m.position << std::endl; } );
	//widget11->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 3: " << m.position << std::endl; } );
	//widget11->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 3: " << m.position << std::endl; } );
	auto widget12 = app->create< Widget >();
	setDefault(widget12);
	widget12->parent = widget1;
	widget12->isResizeable = true;
	//widget12->mouseMoved.changed( [](MouseEvent m) { std::cout << "Widget 4: " << m.position << std::endl; } );
	//widget12->pressed.changed( [](MouseEvent m) { std::cout << "Pressed: Widget 4: " << m.position << std::endl; } );
	//widget12->release.changed( [](MouseEvent m) { std::cout << "Released: Widget 4: " << m.position << std::endl; } );
	window->update();

	auto p = [&](MouseEvent)
	{
	printWidget(window);
	printWidget(widget1);
	printWidget(widget2);
	printWidget(widget11);
	printWidget(widget12);
	};

	window->click.changed(p);

	app->exec();

}

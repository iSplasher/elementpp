/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "include/Window.h"
#include "include/Utils/Primitives.h"

USING_NAMESPACE
//
//class MyWidget : public gCoreWidget {
//public:
//	MyWidget(gCoreWidget* parent = nullptr) : gCoreWidget(parent){
//	}
//};

int main() {
	//gApplication app;
	//gCoreWidget *window =  new gWindow{};
	//MyWidget* c_widget = new MyWidget(window);

	//gCoreWidget *widget2 = new gCoreWidget(widget);
	//gCoreWidget *widget3 = new gCoreWidget(widget);
	//gCoreWidget *widget4 = new gCoreWidget(widget);
	//gCoreWidget *widget5 = new gCoreWidget{};
	//gCoreWidget *widget6 = new gCoreWidget(widget5);
	//gCoreWidget *widget7 = new gCoreWidget(widget5);
	//gCoreWidget *widget8 = new gCoreWidget(widget7);
	//gCoreWidget *widget9 = new gCoreWidget(widget7);
	//gCoreWidget *widget10 = new gCoreWidget(widget9);
	//gCoreWidget *widget11 = new gCoreWidget(widget4);

	//app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));
	//printf("%d", app.core_objects->tree_size());


	//return app.run();

	gPoint a;
	gPoint b(50, 50);
	gSize c;
	gSize d(100,100);
	gRect e;
	gRect f(10, 10, 20, 20);

	c.width = 100;
	d.height = 100;

	d += 5;
	f += d;

	std::cout << a << std::endl << c << std::endl << e << std::endl << b << std::endl << d << std::endl << f << std::endl;

	f = c;
	std::cout << c << std::endl << f << std::endl;

	gRect g;
	if (e == g)
		std::cout << "true\n";
	else
		std::cout << "false\n";

	return 0;
}


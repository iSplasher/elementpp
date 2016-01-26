/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "include/Core.h"
#include "include/Widget.h"
#include <thread>
#include <chrono>

USING_NAMESPACE

int main() {
	gApplication app;
	printf("first");
	gCoreWidget *widget =  new gCoreWidget{};
	std::this_thread::sleep_for(std::chrono::seconds(5));

	gCoreWidget *widget2 = new gCoreWidget(widget);
	gCoreWidget *widget3 = new gCoreWidget(widget);
	gCoreWidget *widget4 = new gCoreWidget(widget);
	gCoreWidget *widget5 = new gCoreWidget{};
	gCoreWidget *widget6 = new gCoreWidget(widget5);
	gCoreWidget *widget7 = new gCoreWidget(widget5);
	gCoreWidget *widget8 = new gCoreWidget(widget7);
	gCoreWidget *widget9 = new gCoreWidget(widget7);
	gCoreWidget *widget10 = new gCoreWidget(widget9);
	gCoreWidget *widget11 = new gCoreWidget(widget4);

	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));
	printf("%d", app.core_objects->tree_size());

	printf("second");
	std::this_thread::sleep_for(std::chrono::seconds(5));

	printf("\nNew tree:\n");
	delete widget5;
	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));

	printf("third");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete widget;

	return app.run();
}


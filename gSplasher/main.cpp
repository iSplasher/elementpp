/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "include/Core.h"
#include "include/Widget.h"

USING_NAMESPACE

class MyWidget : public gCoreWidget {
public:
	MyWidget(gCoreWidget* parent = nullptr) : gCoreWidget(parent){
	}
};

int main() {
	gApplication app;
	gCoreWidget *window =  new gWindow{};
	MyWidget* c_widget = new MyWidget(window);
	c_widget->resize(100,100);
	c_widget->style.bg_color = Color::Green;

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

	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));
	printf("%d", app.core_objects->tree_size());


	return app.run();
}


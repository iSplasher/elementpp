/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"

USING_NAMESPACE

class MyWidget : public gCoreWidget {
public:
	MyWidget(gCoreWidget* parent = nullptr) : gCoreWidget(parent){
		resize(100, 100);
		move(20,20);
	}

	void paint(gPainter &painter) override {
		gPen p(painter);
		gBrush b(painter);
		p.setColor(gColor(255, 0, 0));
		p.setWidth(3);
		b.setColor(gColor(255, 0, 0));
		painter.drawRect(gRect(0, 0, 30, 30));
	}
};

class MyWidget2 : public gCoreWidget {
public:
	MyWidget2(gCoreWidget* parent = nullptr) : gCoreWidget(parent) {
		resize(50, 50);
	}

	void paint(gPainter &painter) override {
		gPen p(painter);
		gBrush b(painter);
		p.setColor(gColor(0, 255, 0));
		p.setWidth(3);
		b.setColor(gColor(0, 255, 0));
		painter.drawRect(gRect(0, 0, 10, 10));
	}
};

int main() {
	gApplication app;
	gWindow *window =  new gWindow{};
	//gCoreWidget *window2 = new gWindow{};
	MyWidget *mywidget = new MyWidget(window);
	MyWidget2 *mywidget2 = new MyWidget2(mywidget);
	std::cout << mywidget->mapFromParent(gPoint(20, 20)) << std::endl;
	std::cout << mywidget2->mapToParent(gPoint(0, 0)) << std::endl;
	//gCoreWidget *window3 = new gWindow{};
	//gCoreWidget *window4 = new gWindow{};
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

	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));
	//printf("%d", app.core_objects->tree_size());


	return app.run();

	return 0;
}


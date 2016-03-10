/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"
#include "gSplasher/Layouts/BoxLayout.h"

USING_NAMESPACE

class MyWidget : public gCoreWidget {
public:
	MyWidget(gCoreWidget* parent = nullptr) : gCoreWidget(parent){
		resize(50, 50);
		move(20,20);
	}

	void paint(gPainter &painter) override {
		gBrush b(painter);
		b.setColor(gColor(255, 0, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

class MyWidget2 : public gCoreWidget {
public:
	MyWidget2(gCoreWidget* parent = nullptr) : gCoreWidget(parent) {
		resize(50, 50);
		move(20, 20);
	}

	void paint(gPainter &painter) override {
		gPen p(painter);
		gBrush b(painter);
		p.setColor(gColor(0, 255, 0));
		p.setWidth(0.1f);
		b.setColor(gColor(0, 255, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

int main() {
	gApplication app;
	gWindow *window =  new gWindow{};
	//gCoreWidget *window2 = new gWindow{};
	MyWidget *mywidget = new MyWidget(window);
	//MyWidget2 *mywidget2 = new MyWidget2(window);
	//MyWidget *mywidget3 = new MyWidget(window);
	//MyWidget *mywidget4 = new MyWidget(window);
	//MyWidget *mywidget5 = new MyWidget(window);
	//MyWidget *mywidget6 = new MyWidget(window);
	//MyWidget *mywidget7 = new MyWidget(window);
	//MyWidget *mywidget8 = new MyWidget(window);

	gBoxLayout<Orientation::Horizontal> layout(window);
	//gBoxLayout<Orientation::Horizontal> layout2(mywidget);
	layout.add(mywidget);
	//layout.add(mywidget2);
	//layout.add(mywidget3);
	//layout.add(mywidget4);
	//layout.add(mywidget5);
	//layout.add(mywidget6);
	//layout.add(mywidget7);
	//layout.add(mywidget8);

	//std::cout << "mywidget " << mywidget->mapFromGlobal(gPoint(0, 0)) << std::endl;
	//std::cout << "mywidget2 " << mywidget2->mapFromGlobal(gPoint(0, 0)) << std::endl;
	//std::cout << "mywidget3 " << mywidget3->mapFromGlobal(gPoint(0, 0)) << std::endl;
	std::cout << "mywidget " << mywidget->geometry() << std::endl;
	//std::cout << "mywidget2 " << mywidget2->geometry() << std::endl;
	//std::cout << "mywidget3 " << mywidget3->geometry() << std::endl;
	std::cout << "window " << window->geometry() << std::endl;
	std::cout << "layout " << layout.geometry() << std::endl;
	//std::cout << "layout2" << layout2.geometry() << std::endl;
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


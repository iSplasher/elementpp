/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"
#include "gSplasher/Layouts/BoxLayout.h"
#include "gSplasher/Global.h"
#include "gSplasher/Widget.h"

USING_NAMESPACE

class MyWidget : public CoreWidget {
public:
	MyWidget(CoreWidget* parent = nullptr) : CoreWidget(parent){
		resize(50, 50);
		move(20, 20);
	}

	void paint(Painter &painter) override {
		gPen p(painter);
		p.setColor(gColor(0, 255, 0));
		p.setWidth(0.8);
		gBrush b(painter);
		b.setColor(gColor(255, 0, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

class MyWidget2 : public CoreWidget {
public:
	MyWidget2(CoreWidget* parent = nullptr) : CoreWidget(parent) {
		resize(50, 50);
		move(20, 20);
	}

	void paint(Painter &painter) override {
		gPen p(painter);
		gBrush b(painter);
		p.setColor(gColor(255, 0, 0));
		p.setWidth(0.8);
		b.setColor(gColor(0, 255, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

int main() {
	Application app;
	Window *window =  new Window{};
	//CoreWidget *window2 = new Window{};
	MyWidget *mywidget = new MyWidget(window);
	MyWidget2 *mywidget2 = new MyWidget2(window);
	MyWidget *mywidget3 = new MyWidget(window);
	MyWidget2 *mywidget4 = new MyWidget2();
	MyWidget *mywidget5 = new MyWidget();
	//MyWidget *mywidget6 = new MyWidget(window);
	//MyWidget *mywidget7 = new MyWidget(window);
	//MyWidget *mywidget8 = new MyWidget(window);

	gBoxLayout<Orientation::Horizontal> *layout = new gBoxLayout<Orientation::Horizontal>{window};
	gBoxLayout<Orientation::Horizontal> *layout2 = new gBoxLayout<Orientation::Horizontal>;
	layout->add(mywidget);
	layout->add(mywidget2);
	layout->add(mywidget3);
	layout->add(layout2);
	layout2->add(mywidget4);
	layout2->add(mywidget5);
	//layout.add(mywidget6);
	//layout.add(mywidget7);
	//layout.add(mywidget8);

	mywidget4->setFixedHeight(100);
	mywidget4->setFixedWidth(450);
	//std::cout << "mywidget to window " << mywidget->mapToWindow(gPoint(0, 0)) << std::endl;
	//std::cout << "mywidget2 to window " << mywidget2->mapToWindow(gPoint(0, 0)) << std::endl;
	//std::cout << "mywidget3 " << mywidget3->mapFromGlobal(gPoint(0, 0)) << std::endl;
	std::cout << "mywidget " << mywidget->geometry() << std::endl;
	std::cout << "mywidget2 " << mywidget2->geometry() << std::endl;
	std::cout << "mywidget3 " << mywidget3->geometry() << std::endl;
	std::cout << "window " << window->geometry() << std::endl;
	std::cout << "topbar " << window->topBar()->geometry() << std::endl;
	std::cout << "layout " << layout->geometry() << std::endl;
	//std::cout << "layout2" << layout2->geometry() << std::endl;
	//CoreWidget *window3 = new Window{};
	//CoreWidget *window4 = new Window{};
	//MyWidget* c_widget = new MyWidget(window);

	//CoreWidget *widget2 = new CoreWidget(widget);
	//CoreWidget *widget3 = new CoreWidget(widget);
	//CoreWidget *widget4 = new CoreWidget(widget);
	//CoreWidget *widget5 = new CoreWidget{};
	//CoreWidget *widget6 = new CoreWidget(widget5);
	//CoreWidget *widget7 = new CoreWidget(widget5);
	//CoreWidget *widget8 = new CoreWidget(widget7);
	//CoreWidget *widget9 = new CoreWidget(widget7);
	//CoreWidget *widget10 = new CoreWidget(widget9);
	//CoreWidget *widget11 = new CoreWidget(widget4);

	app.print_tree(static_cast<Application::CoreList::const_iterator >(*app.core_objects));
	//printf("%d", app.core_objects->tree_size());


	return app.exec();

	return 0;
}


#include "render.h"
#include <FL/Fl.H>

USING_LIB_NAMESPACE
USING_PRIVATE_LIB_NAMESPACE

void render_all( ComponentInstPtr obj) {
    for (auto &o : obj->components) {
        render_all(o->component->render());
    }
    obj->component->render();
}

int elm::render(const ComponentInstPtr& obj ) {
    render_all(obj);
    auto code = Fl::run();
    return 0;
}

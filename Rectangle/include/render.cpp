#include "render.h"
#include <FL/Fl.H>

USING_LIB_NAMESPACE
USING_PRIVATE_LIB_NAMESPACE

void render_all( const ComponentInstPtr& obj) {
    for (auto &o : obj->components) {
        if (o->component) {
            const auto& c = o->component->render();
            if (c) {
                render_all(c);
            }
        }
    }
    obj->component->render();
}

int elm::render(const ComponentInstPtr& obj ) {
    render_all(obj);
    auto code = Fl::run();
    return 0;
}

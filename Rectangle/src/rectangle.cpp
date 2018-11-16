#include "render.h"
#include "window.h"

using namespace elm;

auto comp(const Props& props) {
    return el<ui::Window>(
        {
                kw("size", {500, 500}),
                kw("show", true)
        });
}

int main(int argc, char **argv) {

    return render(el<ui::Window>(
        {
            kw("size", {500, 500}),
            kw("show", true)
        },
        el<comp>()
        ));
}


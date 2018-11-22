#include "component.h"

USING_LIB_NAMESPACE
USING_PRIVATE_LIB_NAMESPACE


ComponentInstPtr ComponentWrapper::render() const {
    if (type == component) {
        return render_cmp ? render_cmp->render() : nullptr;
    }

    return render_func(props);
}

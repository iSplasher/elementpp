#include "render.h"
#include <FL/Fl.H>

USING_LIB_NAMESPACE
USING_PRIVATE_LIB_NAMESPACE

int elm::render( const std::shared_ptr<LIB_PRIVATE_NAMESPACE::ComponentInstance>& obj ) {
    auto code = Fl::run();
    return 0;
}

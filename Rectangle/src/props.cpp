#include "props.h"

USING_LIB_NAMESPACE

IProps::IProps( const PropsPair &pair ) {
    pmap.insert_or_assign( std::get<0>(pair), std::get<1>(pair));
}

IProps::IProps( const std::initializer_list<PropsPair> &pairs) {
    for (auto &pair : pairs) {
        pmap.insert_or_assign(std::get<0>(pair), std::get<1>(pair));
    }
}

void IProps::update( PropsMap src ) {
    std::swap(pmap, src);
    pmap.merge(src);
}

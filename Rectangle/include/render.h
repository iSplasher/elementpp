#pragma once

#include <type_traits>

#include "component.h"

LIB_NAMESPACE_BEGIN

PRIVATE_LIB_NAMESPACE_BEGIN

using ComponentInstPtr = std::shared_ptr< const LIB_PRIVATE_NAMESPACE::ComponentInstance >;
using ComponentInstPtrList = std::initializer_list< ComponentInstPtr >;

template<typename T>
std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance > el_instances_impl(
    ComponentInstPtrList objs,
    std::shared_ptr<const IProps> props = nullptr) {

    std::shared_ptr<Component> obj;

    if (props) {
        obj = std::make_shared<T>(props);
    } else {
        obj = std::make_shared<T>();
    }

    return std::make_shared<LIB_PRIVATE_NAMESPACE::ComponentInstance>(obj);
}

LIB_NAMESPACE_END

template<typename T, typename ...ArgsT>
auto el(const IProps &props, const ArgsT&... args) {
    auto p = std::make_shared<IProps>(props);
    if (std::is_function<T>::value) {
        return LIB_PRIVATE_NAMESPACE::el_instances_impl<Component>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
    }

    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
}

template<typename T, typename ...ArgsT>
auto el(const PropsPair &proppairs, const ArgsT&... args) {
    auto p = std::make_shared<IProps>(proppairs);
    if (std::is_function<T>::value) {
        return LIB_PRIVATE_NAMESPACE::el_instances_impl<Component>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
    }

    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
}

template<typename T, typename ...ArgsT>
auto el(const ArgsT&... args) {
    auto p = std::make_shared<IProps>();
    if (std::is_function<T>::value) {
        return LIB_PRIVATE_NAMESPACE::el_instances_impl<Component>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
    }

    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ...}, p);
}

//
//using ComponentFunc = LIB_PRIVATE_NAMESPACE::ComponentInstPtr(*)(Props);
//
//template<ComponentFunc T, typename ...ArgsT>
//auto el(const ArgsT&... args) {
//    auto p = std::make_shared<IProps>();
//    if (std::is_function<T>::value) {
//        return LIB_PRIVATE_NAMESPACE::el_instances_impl<Component>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
//    }
//
//    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
//    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<LIB_PRIVATE_NAMESPACE::ComponentInstPtr>(args) ... }, p);
//}


int render(const std::shared_ptr<LIB_PRIVATE_NAMESPACE::ComponentInstance>& obj);

LIB_NAMESPACE_END


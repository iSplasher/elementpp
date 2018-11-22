#pragma once

#include <type_traits>
#include <iostream>

#include "component.h"

LIB_NAMESPACE_BEGIN

PRIVATE_LIB_NAMESPACE_BEGIN

template<typename T>
std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance > el_instances_impl(
    ComponentInstPtrList objs,
    std::shared_ptr<const IProps> props = nullptr) {

    Component* obj;

    if (props) {
        obj = new T(props);
    } else {
        obj = new T();
    }

    std::shared_ptr<ComponentWrapper> obj_wrapper = std::make_shared<ComponentWrapper>(props, obj);

    return std::make_shared<LIB_PRIVATE_NAMESPACE::ComponentInstance>(obj_wrapper, objs);
}

template<typename T>
std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance > el_instances_impl(
    ComponentInstPtrList objs,
    PureFunc render_func,
    std::shared_ptr<const IProps> props = nullptr) {

    std::shared_ptr<ComponentWrapper> obj_wrapper = std::make_shared<ComponentWrapper>(props, render_func);

    return std::make_shared<LIB_PRIVATE_NAMESPACE::ComponentInstance>(obj_wrapper, objs);
}

LIB_NAMESPACE_END

template<typename T, typename ...ArgsT>
auto el(const IProps &props, const ArgsT&... args) {
    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    auto p = std::make_shared<IProps>(props);
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<ComponentInstPtr>(args) ... }, p);
}

template<typename T, typename ...ArgsT>
auto el(const PropsPair &proppairs, const ArgsT&... args) {
    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    auto p = std::make_shared<IProps>(proppairs);
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<ComponentInstPtr>(args) ... }, p);
}

template<typename T, typename ...ArgsT>
auto el(const ArgsT&... args) {
    static_assert(std::is_base_of< Component, T >::value, "Must be same or derived from Component");
    auto p = std::make_shared<IProps>();
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<T>({ static_cast<ComponentInstPtr>(args) ...}, p);
}

template<auto T, typename ...ArgsT>
auto el(const ArgsT&... args) {
    auto p = std::make_shared<IProps>();
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<ComponentWrapper>({ static_cast<ComponentInstPtr>(args) ... }, T, p);
}


int render(const ComponentInstPtr& obj);

LIB_NAMESPACE_END


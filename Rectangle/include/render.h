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

    std::shared_ptr<Component> obj;

    if (props) {
        obj = std::make_shared<T>(props);
    } else {
        obj = std::make_shared<T>();
    }

    return std::make_shared<LIB_PRIVATE_NAMESPACE::ComponentInstance>(obj, objs);
}

template<typename T>
std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance > el_instances_impl(
    ComponentInstPtrList objs,
    PureFunc render_func,
    std::shared_ptr<const IProps> props = nullptr) {

    std::shared_ptr<ComponentBase> obj = std::make_shared<T>(props, render_func);

    return std::make_shared<LIB_PRIVATE_NAMESPACE::ComponentInstance>(obj, objs);
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
    return LIB_PRIVATE_NAMESPACE::el_instances_impl<ComponentBase>({ static_cast<ComponentInstPtr>(args) ... }, T, p);
}


int render(const ComponentInstPtr& obj);

LIB_NAMESPACE_END


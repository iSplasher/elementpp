#pragma once

#include "global.h"

#include <functional>
#include <utility>
#include "props.h"

LIB_NAMESPACE_BEGIN

class Component;
class ComponentWrapper;

PRIVATE_LIB_NAMESPACE_BEGIN
class ComponentInstance;
LIB_NAMESPACE_END

using ComponentInstPtr = std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance >;
using ComponentInstPtrList = std::initializer_list< ComponentInstPtr >;
using PureFunc = std::function<ComponentInstPtr(Props)>;

PRIVATE_LIB_NAMESPACE_BEGIN

class ComponentInstance {

public:

    std::shared_ptr<const ComponentWrapper> component;
    std::vector<ComponentInstPtr> components;

    ComponentInstance(std::shared_ptr< const ComponentWrapper > obj, const ComponentInstPtrList objs) : component(std::move(obj)),
                                                                                               components(objs) {}

    virtual ~ComponentInstance() = default;

    ComponentInstance(const ComponentInstance& other)
        : component(other.component),
        components(other.components) {}

    ComponentInstance(ComponentInstance&& other) noexcept
        : component(std::move(other.component)),
        components(std::move(other.components) ) {}

    ComponentInstance& operator=(const ComponentInstance& other) {
        if (this == &other)
            return *this;
        component = other.component;
        components = other.components;
        return *this;
    }

    ComponentInstance& operator=(ComponentInstance&& other) noexcept {
        if (this == &other)
            return *this;
        component = std::move(other.component);
        components = std::move(other.components);
        return *this;
    }

};

LIB_NAMESPACE_END

class ComponentWrapper {

    Props props;

public:

    enum ComponentType {
        pure,
        component
    };

    ComponentWrapper() = default;

    ComponentWrapper( Props props, Component* cmp) : props( std::move( props ) ) {
        type = component;
        render_cmp = std::unique_ptr<Component>(cmp);
    }

    ComponentWrapper(Props props, const PureFunc& rfunc) : props(std::move(props)) {
        type = pure;
        render_func = rfunc;
    }

    ComponentWrapper(ComponentWrapper&& other) noexcept
        : render_func(std::move(other.render_func)),
        render_cmp(std::move(other.render_cmp)),
        props(std::move(other.props)),
        type(other.type) {}

    ComponentWrapper& operator=( ComponentWrapper&& other ) noexcept {
        if( this == &other )
            return *this;
        render_func = std::move( other.render_func );
        render_cmp = std::move( other.render_cmp);
        props = std::move( other.props );
        type = other.type;
        return *this;
    }

    virtual ~ComponentWrapper() = default;

    ComponentInstPtr render () const;

private:

    PureFunc render_func;
    std::unique_ptr<Component> render_cmp;
    ComponentType type;

};

class Component {

public:
    Props props;

    Component() = default;

    Component( Props props) : props( std::move( props ) ){}


    Component( Component&& other ) noexcept
        : props(std::move(other.props)) {}

    Component& operator=( Component&& other ) noexcept {
        if( this == &other )
            return *this;
        props = std::move(other.props);
        return *this;
    }

    virtual ~Component() = default;

    virtual ComponentInstPtr render() const {
        return nullptr;
    }

    void setState(){}
};

LIB_NAMESPACE_END
#pragma once

#include "global.h"

#include <functional>
#include <utility>
#include "props.h"

LIB_NAMESPACE_BEGIN

class Component;
class ComponentBase;

PRIVATE_LIB_NAMESPACE_BEGIN
class ComponentInstance;
LIB_NAMESPACE_END

using ComponentInstPtr = std::shared_ptr< LIB_PRIVATE_NAMESPACE::ComponentInstance >;
using ComponentInstPtrList = std::initializer_list< ComponentInstPtr >;
using PureFunc = std::function<ComponentInstPtr(Props)>;

PRIVATE_LIB_NAMESPACE_BEGIN

class ComponentInstance {

public:

    std::shared_ptr<const ComponentBase> component;
    ComponentInstPtrList components;

    ComponentInstance() = default;

    ComponentInstance(std::shared_ptr< const ComponentBase > obj, ComponentInstPtrList objs) : component(std::move(obj)),
                                                                                               components(objs) {}

    virtual ~ComponentInstance() = default;

    ComponentInstance(const ComponentInstance& other)
        : component(other.component),
        components(other.components) {}

    ComponentInstance(ComponentInstance&& other) noexcept
        : component(std::move(other.component)),
        components( other.components ) {}

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
        components = other.components;
        return *this;
    }

};

LIB_NAMESPACE_END

class ComponentBase {

public:
    Props props;

    enum ComponentType {
        pure,
        component
    };

    ComponentBase() = default;

    ComponentBase( Props props, Component* cmp) : props( std::move( props ) ) {
        type = component;
        render_cmp = std::unique_ptr<Component>(cmp);
    }

    ComponentBase(Props props, const PureFunc& rfunc) : props(std::move(props)) {
        type = pure;
        render_func = rfunc;
    }

    ComponentBase(ComponentBase&& other) noexcept
        : render_func(std::move(other.render_func)),
        render_cmp(std::move(other.render_cmp)),
        props(std::move(other.props)),
        type(other.type) {}

    ComponentBase& operator=( ComponentBase&& other ) noexcept {
        if( this == &other )
            return *this;
        render_func = std::move( other.render_func );
        render_cmp = std::move( other.render_cmp);
        props = std::move( other.props );
        type = other.type;
        return *this;
    }

    virtual ~ComponentBase() = default;

    virtual ComponentInstPtr render () const;

private:

    PureFunc render_func;
    std::unique_ptr<Component> render_cmp;
    ComponentType type;

};

class Component : public ComponentBase {

public:
    Component() = default;

    Component(const Props &props) : ComponentBase(props, this){}


    Component( Component&& other ) noexcept
        : ComponentBase( std::move(other) ) {}

    Component& operator=( Component&& other ) noexcept {
        if( this == &other )
            return *this;
        ComponentBase::operator =( std::move( other ) );
        return *this;
    }

    virtual ~Component() = default;

    ComponentInstPtr render() const override {
        return ComponentBase::render();
    }

    void setState(){}
};

LIB_NAMESPACE_END
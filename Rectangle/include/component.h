#pragma once

#include "global.h"

#include "props.h"

LIB_NAMESPACE_BEGIN

class Component {

    Props props;
    //state

public:

    Component() = default;

    Component( const Component& other )
        : props( other.props ) {}

    Component( Component&& other ) noexcept
        : props( std::move(other.props) ) {}

    Component& operator=( const Component& other ) {
        if( this == &other )
            return *this;
        props = other.props;
        return *this;
    }

    Component& operator=( Component&& other ) noexcept {
        if( this == &other )
            return *this;
        props = std::move( other.props );
        return *this;
    }

    Component(const Props &props) : props(props) {}

    virtual ~Component() = default;

    virtual void render() {
        
    }

    void setState(){}

};


PRIVATE_LIB_NAMESPACE_BEGIN

class ComponentInstance {

    std::shared_ptr<const Component> component;

public:

    ComponentInstance() = default;

    ComponentInstance( std::shared_ptr< const Component > obj) : component( std::move( obj ) ) {}

    virtual ~ComponentInstance() = default;

    ComponentInstance(const ComponentInstance& other)
        : component(other.component) {}

    ComponentInstance(ComponentInstance&& other) noexcept
        : component(std::move(other.component)) {}

    ComponentInstance& operator=(const ComponentInstance& other) {
        if (this == &other)
            return *this;
        component = other.component;
        return *this;
    }

    ComponentInstance& operator=(ComponentInstance&& other) noexcept {
        if (this == &other)
            return *this;
        component = std::move(other.component);
        return *this;
    }


};

LIB_NAMESPACE_END


LIB_NAMESPACE_END
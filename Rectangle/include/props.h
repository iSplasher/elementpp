#pragma once

#include "global.h"
#include <unordered_map>
#include <string_view>
#include <any>
#include <functional>
#include <optional>

LIB_NAMESPACE_BEGIN

struct IProps;

using Props = std::shared_ptr<const IProps>;
using AnyProp = std::any;
using PropsMap = std::unordered_map<std::string_view, AnyProp>;
using PropsPair = std::tuple<std::string_view, AnyProp>;

struct IProps {

    IProps() = default;
    IProps(const PropsPair&);
    IProps(const std::initializer_list<PropsPair>&);
    IProps(const Props &props) : IProps( *props ) {}

    void update(PropsMap src);

    operator Props() const { return std::make_shared<const IProps>(*this); }

    template<typename T>
    std::optional<T> get( const std::string_view name) const {
        const auto it = pmap.find(name);
        if (it != pmap.end()) {
            return std::any_cast<T>(it->second);
        }
        return std::nullopt;
    }

private:
    PropsMap pmap;

};


inline PropsPair kw(const std::string_view name, const AnyProp &value) {
    return { name, value };
}

inline PropsPair kw(const std::string_view name, const std::pair<double, double> &value) {
    return { name, value };
}

inline PropsPair kw(const std::string_view name, const std::pair<std::string_view, std::string_view> &value) {
    return { name, value };
}

LIB_NAMESPACE_END

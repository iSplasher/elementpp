#pragma once

#include <type_traits>
#include <functional>

#include "react/Domain.h"
#include <react/Signal.h>

#include "gSplasher/Global.h"

NAMESPACE_BEGIN

PRIV_NAMESPACE_BEGIN

struct D : public REACT_IMPL::DomainBase<D, REACT_IMPL::DomainPolicy< react::sequential >> {};
static REACT_IMPL::DomainInitializer<D> D_initializer_;

template <typename S>
using SignalT = react::Signal<D, S>;
template <typename S>
using VarSignalT = react::VarSignal<D, S>;
template <typename E = react::Token>
using EventsT = react::Events<D, E>;
template <typename E = react::Token>
using EventSourceT = react::EventSource<D, E>;
using ObserverT = react::Observer<D>;
using ScopedObserverT = react::ScopedObserver<D>;
using ReactorT = react::Reactor<D>;

NAMESPACE_END

enum PropertyType {
	Read,
	Write
};

/// <summary>
/// Base Property
/// </summary>
template<typename T, PropertyType pType = Write>
class Property
{
};

/// <summary>
/// Read only property specialization
/// </summary>
template<typename T>
class Property<T, Read>
{
	using func = std::function<T&(const T &)>;
	using Reactive = PRIV_NAMESPACE::SignalT<T>;
public:
	Property() {}
	Property(func get_func = nullptr) : Property(T(), get_func) {}

	/// <summary>
	/// read only CTOR
	/// </summary>
	/// <param name="value"></param>
	/// <param name="get_func"></param>
	Property(const T &value, func get_func = nullptr)
	{
		this->value = value;
		this->get_func = get_func;
	}


private:
	Reactive reactive;

	func get_func = nullptr;

	friend std::ostream& operator<<(std::ostream& os, const Property<T, Read>& obj)
	{
		os << obj.reactive.Value();
		return os;
	}
};

/// <summary>
/// Read-write property specialization
/// </summary>
template<typename T>
class Property<T, Write>
{
	using Reactive = PRIV_NAMESPACE::VarSignalT<T>;
	using func = std::function<void(T)>;
public:
	Property() : reactive(react::MakeVar<PRIV_NAMESPACE::D>(T())) {
	}

	Property(const T value) : reactive(react::MakeVar<PRIV_NAMESPACE::D>(value))
	{
	}

	/// <summary>
	/// property = value
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	auto operator=(T value) {
		reactive <<= value;
		return *this;
	}

	/// <summary>
	/// bool(property)
	/// </summary>
	/// <returns></returns>
	//operator bool() const {
	//	return reactive.Value() ? true : false;

	//}

	/// <summary>
	/// property->member
	/// </summary>
	/// <returns></returns>
	T operator->() const
	{
		return reactive.Value();
	}

	/// <summary>
	/// typecast operator
	/// </summary>
	/// <returns></returns>
	operator T() const {
		return reactive.Value();
	}

	void connect(func f)
	{
		react::Observe(reactive, f);
	}

private:
	Reactive reactive;

	friend std::ostream& operator<<(std::ostream& os, const Property<T, Write>& obj)
	{
		os << obj.reactive.Value();
		return os;
	}
};




NAMESPACE_END
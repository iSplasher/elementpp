#pragma once

#include "gSplasher/Global.h"
#include "gSplasher/Property/_emitter.h"
#include "gSplasher/Property/_utils.h"

#include <tuple>
#include <vector>
#include <iostream>

NAMESPACE_BEGIN

PRIV_NAMESPACE_BEGIN

namespace detail {

	struct property_base {
		virtual ~property_base() {
			aboutToDestruct.fire();
		}

		emitter<> aboutToDestruct;
	};

	struct binding_base {
		virtual ~binding_base() {
			for (auto &c : std::move(connections))
				c.disconnect();
		}

		std::vector<emitter_base::connection> connections;
	};

}

NAMESPACE_END

template <class T>
struct property;

template <class T>
struct binding;

template <class T>
struct property : PRIV_NAMESPACE::detail::property_base
{
	PRIV_NAMESPACE::emitter<T> onChanged;

	property &operator=(const T &t) {
		value = t;
		onChanged.fire(value);
		return *this;
	}

	operator T () const {
		return value;
	}

	template <class Func, class... Args,
		class Result = std::result_of_t<Func(Args...)>,
		class = std::enable_if_t<std::is_convertible<Result, T>::value>>
		void bind(Func&& f, Args&&... args)
	{
		auto b = binding<Result>::create(std::forward<Func>(f), std::forward<Args>(args)...);

		b->onUpdated.connect([this](const Result &r) {
			*this = r;
		});

		b->connections.emplace_back(aboutToDestruct.connect([b] {
			delete b;
		}));

		b->update();
	}

private:
	T value{};
};

template <class T>
struct binding : PRIV_NAMESPACE::detail::binding_base
{
private:
	template <class Func, class... Args>
	binding(Func&& f, Args&&... args)
	{
		func = [f = std::move(f), a = std::tuple<Args...>(args...)]() mutable -> decltype(f(args...)) {
			return PRIV_NAMESPACE::utils::invoke_tuple(std::forward<decltype(f)>(f),
				std::forward<decltype(a)>(a));
		};

		listen(args...);
	}

public:

	template <class... Args>
	static inline binding<T> *
		create(Args&&... args)
	{
		return new binding(std::forward<Args>(args)...);
	}

	void listen() {}

	template <class U, class... Tail,
		class = std::enable_if_t<std::is_base_of<PRIV_NAMESPACE::detail::property_base, U>::value == false>>
		void listen(U &, Tail&&... tail)
	{
		listen(std::forward<Tail>(tail)...);
	}

	template <class U, class... Tail>
	void listen(property<U>& head, Tail&&... tail)
	{
		connections.emplace_back(head.onChanged.connect([this] {
			update();
		}));

		connections.emplace_back(head.aboutToDestruct.connect([this] {
			std::cerr << "Broken binding due to destruction of dependent property!" << std::endl;
			delete this;
		}));

		listen(std::forward<Tail>(tail)...);
	}

	void update()
	{
		onUpdated.fire(func());
	}

	std::function<T()> func;
	PRIV_NAMESPACE::emitter<T> onUpdated;
};

NAMESPACE_END



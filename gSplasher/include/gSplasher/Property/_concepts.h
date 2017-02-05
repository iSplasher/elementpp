#pragma once

#include "gSplasher/Global.h"
#include "gSplasher/Property/_utils.h"

NAMESPACE_BEGIN

PRIV_NAMESPACE_BEGIN

namespace concepts {

	struct Callable {
		template <class F, class... Args>
		auto requires_(F &&f, Args&&... args)
			-> decltype(f(std::forward<Args>(args)...));
	};

	template <class Concept, class Enable = void>
	struct models : std::false_type {};

	template <class Concept, class... Ts>
	struct models
		< Concept(Ts...)
		, utils::void_t<decltype(std::declval<Concept>().requires_(std::declval<Ts>()...))>
		> : std::true_type {};

} //  concepts

NAMESPACE_END // priv

NAMESPACE_END

#define MODELS_CONCEPT(...) NAMESPACE::PRIV_NAMESPACE::concepts::models<__VA_ARGS__>::value
#define REQUIRES_CONCEPT(...) typename std::enable_if<NAMESPACE::PRIV_NAMESPACE::concepts::models<__VA_ARGS__>::value, int>::type = 0


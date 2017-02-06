#pragma once

#include <type_traits>
#include <functional>
#include <initializer_list>

#include <react/Domain.h>
#include <react/Signal.h>

#include "gSplasher/Global.h"

NAMESPACE_BEGIN
PRIV_NAMESPACE_BEGIN
struct D : public ::react::impl::DomainBase< D, ::react::impl::DomainPolicy< react::sequential_concurrent > > {};


static ::react::impl::DomainInitializer< D > D_initializer_;

template< typename S >
using SignalT = react::Signal< D, S >;
template< typename S >
using VarSignalT = react::VarSignal< D, S >;
template< typename E = react::Token >
using EventsT = react::Events< D, E >;
template< typename E = react::Token >
using EventSourceT = react::EventSource< D, E >;
using ObserverT = react::Observer< D >;
using ScopedObserverT = react::ScopedObserver< D >;
using ReactorT = react::Reactor< D >;

NAMESPACE_END


enum PropertyType {
	Read,
	Write
};


enum ConnectionReleasePolicy {
	Immediate,
	Permanent
};


template< typename T, ConnectionReleasePolicy R = Permanent >
class Connection {
public:
	Connection( const Connection& other )
		: prop( other.prop ),
		  status( other.status ),
		  obs( other.obs ),
		  released( other.released ) {}

	Connection( Connection&& other ) noexcept
		: prop( other.prop ),
		  status( other.status ),
		  obs( std::move( other.obs ) ),
		  released( other.released ) {}

	Connection& operator=( const Connection& other );

	Connection& operator=( Connection&& other ) noexcept;

	Connection( T&, PRIV_NAMESPACE::ObserverT, react::TransactionStatus& );

	~Connection();

	/// <summary>
	/// Disconnect from property
	/// </summary>
	bool release();

	/// <summary>
	/// Check if connection is linked to property
	/// </summary>
	bool isReleased() const { return released; }; // TODO: maybe property?

private:

	T& prop;
	react::TransactionStatus& status;
	PRIV_NAMESPACE::ObserverT obs;
	bool released = false;

};


/// <summary>
/// Base Property
/// </summary>
template< typename T, PropertyType pType = Write >
class Property {};


/// <summary>
/// Read only property specialization
/// </summary>
template< typename T >
class Property< T, Read > {
	using func = std::function< T&( const T& ) >;
	using Reactive = PRIV_NAMESPACE::SignalT< T >;
public:
	Property() {}
	Property( func get_func = nullptr ) : Property( T(), get_func ) {}

	/// <summary>
	/// read only CTOR
	/// </summary>
	/// <param name="value"></param>
	/// <param name="get_func"></param>
	Property( const T& value, func get_func = nullptr ) {
		this->value = value;
		this->get_func = get_func;
	}


private:
	Reactive reactive;

	func get_func = nullptr;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Read >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/// <summary>
/// Read-write property specialization
/// </summary>
template< typename T >
class Property< T, Write > {
	using Reactive = PRIV_NAMESPACE::VarSignalT< T >;
	using func = std::function< void( T ) >;

public:
	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	/// <summary>
	/// property = value
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	auto operator=( T value ) {
		reactive <<= std::forward< T >( value );
		return *this;
	}

	/// <summary>
	/// property->member
	/// </summary>
	/// <returns></returns>
	T operator->() { return reactive.Value(); }

	/// <summary>
	/// typecast operator
	/// </summary>
	/// <returns></returns>
	operator T() const { return reactive.Value(); }

	/**
	 * \brief Property << value \n
	 * This operator will append the value asynchronously
	 * \param value T
	 */
	void operator<<( T value ) { react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [&] { reactive <<= std::forward< T >( value ); } ); }

	// FUNCTIONS

	template< ConnectionReleasePolicy C = Permanent >
	Connection< Property< T >, C > connect( func f ) { return Connection< Property< T >, C >( this, react::MakeContinuation( reactive, f ), status ); }


	// Relational operators

	bool operator==( const T& rhs ) { return ( rhs == reactive.Value() ); }

	bool operator==( const Property< T >& rhs ) { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) { return !( rhs == reactive.Value() ); }

	bool operator!=( const Property< T >& rhs ) { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) { return ( rhs < reactive.Value() ); }

	bool operator<( const Property< T >& rhs ) { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) { return ( rhs > reactive.Value() ); }

	bool operator>( const Property< T >& rhs ) { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) { return ( rhs >= reactive.Value() ); }

	bool operator>=( const Property< T >& rhs ) { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) { return ( rhs <= reactive.Value() ); }

	bool operator<=( const Property< T >& rhs ) { return ( rhs.reactive.Value() <= reactive.Value() ); }

private:
	Reactive reactive;
	react::TransactionStatus status;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Write >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


NAMESPACE_END

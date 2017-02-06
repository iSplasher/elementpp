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


enum ConnectionType {
	/**
	 * \brief Create a temporary connection that will only run once
	 */
	Temporary,
	/**
	 * \brief Create a permanent connection that will be permanently connected to handler
	 */
	Permanent
};


/**
 * \brief A connection object returned when connecting property to functions
 * \tparam T Property<T>
 * \tparam R Connection release policy
 */
template< typename T > // TODO: static assert T is Property object
class Connection {
public:

	Connection( T* t, react::TransactionStatus& s, PRIV_NAMESPACE::ObserverT&& o ) : prop( t ),
	                                                                                 status( s ),
	                                                                                 obs( std::move( o ) ) {}

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

	Connection& operator=( const Connection& other ) {
		if( this == &other )
			return *this;
		prop = other.prop;
		status = other.status;
		obs = other.obs;
		released = other.released;
		return *this;
	}

	Connection& operator=( Connection&& other ) noexcept {
		if( this == &other )
			return *this;
		prop = other.prop;
		status = other.status;
		obs = std::move( other.obs );
		released = other.released;
		return *this;
	}

	/**
	 * \brief Disconnect from property
	 * \return If release was successful or not
	 */
	bool release() {
		if( obs.IsValid() )
			obs.Detach();
		released = !obs.IsValid();
		return released;
	};

	/**
	 * \brief Check if connection is linked to property
	 * \return 
	 */
	bool isReleased() const { return released; }; // TODO: maybe property?

private:

	T* prop;
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


/**
 * \brief Read-write property specialization
 * \tparam T 
 */
template< typename T >
class Property< T, Write > {
	using Reactive = PRIV_NAMESPACE::VarSignalT< T >;
	using func = std::function< void( T ) >;

public:

	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	/**
	 * \brief property = value
	 * \param value T
	 * \return 
	 */
	auto& operator=( T value ) {
		reactive <<= std::forward< T >( value );
		return *this;
	}

	/**
	 * \brief property->member
	 * \return 
	 */
	T operator->() { return reactive.Value(); }


	/**
	 * \brief typecast operator
	 */
	operator T() const { return reactive.Value(); }

	/**
	 * \brief Property << value. This operator will append the value asynchronously
	 * \param value T
	 * \see wait()
	 */
	void operator<<( T value ) { react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [&] { reactive <<= std::forward< T >( value ); } ); }

	/**
	 * \brief Property << {value, value, ...}. This operator will append the value asynchronously
	 * \param value T
	 * \see wait()
	 */
	void operator<<( std::initializer_list< T > value ) {
		react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [&] {
			                                              for( auto& x : value ) {
				                                              reactive <<= std::forward< T >( x );
			                                              }
		                                              } );
	}

	// FUNCTIONS

	/**
	 * \brief 
	 * \tparam C connection release policy
	 * \param f function to connect
	 * \return 
	 */
	template< ConnectionType C = Permanent >
	auto connect( func f ) {
		PRIV_NAMESPACE::ObserverT obs;

		switch( C ) {
			case Permanent:
			{
				obs = std::move( react::Observe( reactive, f ) );
				break;
			}
			case Temporary:
			{ 
				// only run once
				std::move( react::Observe( reactive, [=](T t) -> react::ObserverAction {
					                           f( std::forward< T >( t ) );
					                           return react::ObserverAction::stop_and_detach;
				                           } ) );
				break;
			}
		}

		return Connection< Property< T, Write > >(this, status, std::move(obs));
	}


	/**
	 * \brief Wait for all computations have been made
	 * \remark Equivalent to a thread.join()
	 * \see operator<<()
	 */
	void wait() { status.Wait(); }

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

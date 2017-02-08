#pragma once

#include <type_traits>
#include <functional>
#include <initializer_list>

#include <react/Domain.h>
#include <react/Signal.h>

#include "gSplasher/Global.h"
#include <GL/glew.h>

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
	Permanent,

	/**
	* \brief Create a permanent connection that will be released when the connection object goes out of scope
	*/
	Scoped
};


/**
 * \brief A connection object returned when connecting property to functions
 * \tparam T Property<T>
 */
template< typename T > // TODO: static assert T is Property object
class Connection {
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;
public:

	Connection( const Connection& other )
		: scoped( other.scoped ),
		  prop( other.prop ),
		  status( other.status ),
		  cont( other.cont ) {}

	Connection( Connection&& other ) noexcept
		: scoped( other.scoped ),
		  prop( other.prop ),
		  status( other.status ),
		  cont( other.cont ) {}

	Connection& operator=( const Connection& other ) {
		if( this == &other )
			return *this;
		scoped = other.scoped;
		prop = other.prop;
		status = other.status;
		cont = other.cont;
		return *this;
	}

	Connection& operator=( Connection&& other ) noexcept {
		if( this == &other )
			return *this;
		scoped = other.scoped;
		prop = other.prop;
		status = other.status;
		cont = other.cont;
		return *this;
	}

	~ Connection() {
		if( scoped )
			release();
	}

	/**
	 * \brief Disconnect from property
	 */
	void release() const {
		if( cont ) {
			cont.reset();
		}
	};

	/**
	 * \brief Check if connection is linked to property
	 * \return 
	 */
	bool isReleased() const { return cont ? false : true; }; // TODO: maybe property?

private:

	Connection( T* t, react::TransactionStatus& s, std::unique_ptr< Continuation >& o, bool scope = false ) : scoped( scope ),
	                                                                                                          prop( t ),
	                                                                                                          status( s ),
	                                                                                                          cont( o ) {}

	bool scoped;
	T* prop;
	react::TransactionStatus& status;
	std::unique_ptr< Continuation >& cont;

	friend typename T;
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
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	Property( const Property& other )
		: continuations( other.continuations ),
		  temporary_connections( other.temporary_connections ),
		  reactive( other.reactive ),
		  status( other.status ) {}

	Property( Property&& other ) noexcept
		: continuations( std::move( other.continuations ) ),
		  temporary_connections( std::move( other.temporary_connections ) ),
		  reactive( std::move( other.reactive ) ),
		  status( std::move( other.status ) ) {}

	Property& operator=( const Property& other ) {
		if( this == &other )
			return *this;
		continuations = other.continuations;
		temporary_connections = other.temporary_connections;
		reactive = other.reactive;
		status = other.status;
		return *this;
	}

	Property& operator=( Property&& other ) noexcept {
		if( this == &other )
			return *this;
		continuations = std::move( other.continuations );
		temporary_connections = std::move( other.temporary_connections );
		reactive = std::move( other.reactive );
		status = std::move( other.status );
		return *this;
	}

	~Property() {
		for( auto& p : continuations ) {
			if( p ) {
				p.reset();
			}
		}
	}

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
	void operator<<( T value ) { react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [=] { reactive <<= value; } ); }

	/**
	 * \brief Property << {value, value, ...}. This operator will append the value asynchronously
	 * \param value T
	 * \see wait()
	 */
	void operator<<( std::initializer_list< T > value ) {
		react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [=] {
			                                              for( auto& x : value ) {
				                                              reactive <<= x;
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
		std::unique_ptr< Continuation > _cont = nullptr;
		continuations.push_back( std::move( _cont ) );
		auto& cont = continuations.back();

		switch( C ) {
			case Scoped:
			case Permanent:
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, f ) ) ) );
				break;
			}
			case Temporary: // run only once
			{

				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, [=, &cont](T t) {
					                                                             f( std::forward< T >( t ) );
																				 if (cont)
																					 cont.reset();
				                                                             } ) ) ));
				break;
			}
		}
		return Connection< Property< T, Write > >( this, status, cont, C == Scoped );
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
	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Write >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


NAMESPACE_END

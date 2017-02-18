#pragma once

#include "global.h"

#include <react/Domain.h>
#include <react/Signal.h>

#include <type_traits>
#include <functional>
#include <initializer_list>
#include <list>

NAMESPACE_BEGIN
enum class ConnectionType {
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


struct PropertyEventType {};


/**
* \brief Create a read-only property that acts like a view for other properties
*/
struct PropertyViewType {};


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


/**
 * \brief Private connection object returned when connecting property to functions
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

	friend T;
};


class Element;

template< typename T, typename Private >
class Property;

template< typename T >
class Property< T, Element >;

template< typename T >
class Property< T, PropertyViewType >;

template< typename T >
using PropertyView = Property< T, PropertyViewType >;


/**
 * \brief Private read-only property
 * \tparam T type
 * \tparam Private class allowed to access this property
 */
template< typename T, typename Private = Element >
class Property {
	using Reactive = PRIV_NAMESPACE::VarSignalT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	/**
	* \brief property->member
	* \return
	*/
	T operator->() { return reactive.Value(); }


	/**
	* \brief typecast operator
	*/
	operator T() const { return reactive.Value(); }

	// FUNCTIONS

	/**
	* \brief
	* \tparam C connection release policy
	* \param f function to changed
	* \return
	*/
	template< ConnectionType C = ConnectionType::Permanent >
	auto changed( func f ) {
		std::unique_ptr< Continuation > _cont = nullptr;
		continuations.push_back( std::move( _cont ) );
		auto& cont = continuations.back();

		switch( C ) {
			case ConnectionType::Scoped:
			case ConnectionType::Permanent:
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, f ) ) ) );
				break;
			}
			case ConnectionType::Temporary: // run only once
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, [=, &cont](T t) {
					                                                                  f( std::forward< T >( t ) );
					                                                                  if( cont )
						                                                                  cont.reset();
				                                                                  } ) ) ) );
				break;
			}
		}
		return Connection< Property< T, Private > >( this, status, cont, C == ConnectionType::Scoped );
	}


	/**
	* \brief Wait for all computations have been made
	* \remark Equivalent to a thread.join()
	* \see operator<<()
	*/
	void wait() { status.Wait(); }

	/**
	 * \brief Get T
	 * \return T
	 */
	T get() { return this; }

	// Relational operators

	bool operator==( const T& rhs ) { return ( rhs == reactive.Value() ); }

	bool operator==( const Property< T, Private >& rhs ) { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) { return !( rhs == reactive.Value() ); }

	bool operator!=( const Property< T, Private >& rhs ) { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) { return ( rhs < reactive.Value() ); }

	bool operator<( const Property< T, Private >& rhs ) { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) { return ( rhs > reactive.Value() ); }

	bool operator>( const Property< T, Private >& rhs ) { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) { return ( rhs >= reactive.Value() ); }

	bool operator>=( const Property< T, Private >& rhs ) { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) { return ( rhs <= reactive.Value() ); }

	bool operator<=( const Property< T, Private >& rhs ) { return ( rhs.reactive.Value() <= reactive.Value() ); }

private:

	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args&& ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	explicit Property( T&& t ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( std::forward< T >( t ) ) ) { }

	~Property() {
		for( auto& p : continuations ) {
			if( p ) {
				p.reset();
			}
		}
	}

	// MOVE & COPY

	Property(const Property& other) = delete;
	Property& operator=(const Property& other) = delete;


	Property( Property&& other ) noexcept
		: continuations( std::move( other.continuations ) ),
		  reactive( std::move( other.reactive ) ),
		  status( std::move( other.status ) ) {}


	Property& operator=( Property&& other ) noexcept {
		if( this == &other )
			return *this;
		continuations = std::move( other.continuations );
		reactive = std::move( other.reactive );
		status = std::move( other.status );
		return *this;
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

	/**
	* \brief Property << value. This operator will append the value asynchronously
	* \param value T
	* \see wait()
	*/
	void operator<<( T value ) { react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [=] { reactive <<= value; } ); }

	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	friend Private;
	template< typename P, typename PropertyViewType >
	friend class Property;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Private >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/**
 * \brief A read-write property
 * \tparam T 
 */
template< typename T >
class Property< T, Element > {
	using Reactive = PRIV_NAMESPACE::VarSignalT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args&& ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	explicit Property( T&& t ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( std::forward< T >( t ) ) ) { }

	Property(const Property& other) = delete;

	Property( Property&& other ) noexcept
		: continuations( std::move( other.continuations ) ),
		  reactive( std::move( other.reactive ) ),
		  status( std::move( other.status ) ) {}

	Property& operator=(const Property& other) = delete;
	Property& operator=( Property&& other ) noexcept {
		if( this == &other )
			return *this;
		continuations = std::move( other.continuations );
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
	 * \brief Construct a connection to a function that will be called on every property change
	 * \tparam C connection release policy
	 * \param f function to changed
	 * \return 
	 */
	template< ConnectionType C = ConnectionType::Permanent >
	auto changed( func f ) {
		std::unique_ptr< Continuation > _cont = nullptr;
		continuations.push_back( std::move( _cont ) );
		auto& cont = continuations.back();

		switch( C ) {
			case ConnectionType::Scoped:
			case ConnectionType::Permanent:
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, f ) ) ) );
				break;
			}
			case ConnectionType::Temporary: // run only once
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, [=, &cont](T t) {
					                                                                  f( std::forward< T >( t ) );
					                                                                  if( cont )
						                                                                  cont.reset();
				                                                                  } ) ) ) );
				break;
			}
		}
		return Connection< Property< T, Element > >( this, status, cont, C == ConnectionType::Scoped );
	}


	/**
	 * \brief Wait for all async computations have been made
	 * \remark Equivalent to a thread.join()
	 * \see operator<<()
	 */
	void wait() { status.Wait(); }

	/**
	* \brief Get T
	* \return T
	*/
	T get() { return reactive.Value(); }

	// Boolean operators

	bool operator!() const { return !get(); }

	// Relational operators

	bool operator==( const T& rhs ) const { return ( rhs == reactive.Value() ); }

	bool operator==( const Property< T, Element >& rhs ) const { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) const { return !( rhs == reactive.Value() ); }

	bool operator!=( const Property< T, Element >& rhs ) const { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) const { return ( rhs < reactive.Value() ); }

	bool operator<( const Property< T, Element >& rhs ) const { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) const { return ( rhs > reactive.Value() ); }

	bool operator>( const Property< T, Element >& rhs ) const { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) const { return ( rhs >= reactive.Value() ); }

	bool operator>=( const Property< T, Element >& rhs ) const { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) const { return ( rhs <= reactive.Value() ); }

	bool operator<=( const Property< T, Element >& rhs ) const { return ( rhs.reactive.Value() <= reactive.Value() ); }

private:
	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	template< typename P, typename PropertyViewType >
	friend class Property;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Element >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/**
* \brief A property view
* \tparam T type
*/
template< typename T >
class Property< T, PropertyViewType > {
	using Reactive = PRIV_NAMESPACE::SignalT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	template< typename F, typename... Args >
	explicit Property( F&& f, Args&&... args ) {
		// TODO: find a way to static_assert function arguments count
		reactive = react::MakeSignal( react::With( args.reactive ... ), f );
	}

	~Property() {
		for( auto& p : continuations ) {
			if( p ) {
				p.reset();
			}
		}
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

	// FUNCTIONS

	/**
	* \brief
	* \tparam C connection release policy
	* \param f function to changed
	* \return
	*/
	template< ConnectionType C = ConnectionType::Permanent >
	auto changed( func f ) {
		std::unique_ptr< Continuation > _cont = nullptr;
		continuations.push_back( std::move( _cont ) );
		auto& cont = continuations.back();

		switch( C ) {
			case ConnectionType::Scoped:
			case ConnectionType::Permanent:
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, f ) ) ) );
				break;
			}
			case ConnectionType::Temporary: // run only once
			{
				cont.reset( new Continuation( std::move( react::MakeContinuation( reactive, [=, &cont](T t) {
					                                                                  f( std::forward< T >( t ) );
					                                                                  if( cont )
						                                                                  cont.reset();
				                                                                  } ) ) ) );
				break;
			}
		}
		return Connection< Property< T, PropertyViewType > >( this, status, cont, C == ConnectionType::Scoped );
	}


	/**
	* \brief Wait for all computations have been made
	* \remark Equivalent to a thread.join()
	* \see operator<<()
	*/
	void wait() { status.Wait(); }

	/**
	* \brief Get T
	* \return T
	*/
	T get() { return this; }

	// Relational operators

	bool operator==( const T& rhs ) { return ( rhs == reactive.Value() ); }

	bool operator==( const PropertyView< T >& rhs ) { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) { return !( rhs == reactive.Value() ); }

	bool operator!=( const PropertyView< T >& rhs ) { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) { return ( rhs < reactive.Value() ); }

	bool operator<( const PropertyView< T >& rhs ) { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) { return ( rhs > reactive.Value() ); }

	bool operator>( const PropertyView< T >& rhs ) { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) { return ( rhs >= reactive.Value() ); }

	bool operator>=( const PropertyView< T >& rhs ) { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) { return ( rhs <= reactive.Value() ); }

	bool operator<=( const PropertyView< T >& rhs ) { return ( rhs.reactive.Value() <= reactive.Value() ); }

private:

	// MOVE & COPY

	Property( const Property& other )
		: continuations( other.continuations ),
		  reactive( other.reactive ),
		  status( other.status ) {}

	Property( Property&& other ) noexcept
		: continuations( std::move( other.continuations ) ),
		  reactive( std::move( other.reactive ) ),
		  status( std::move( other.status ) ) {}

	Property& operator=( const Property& other ) {
		if( this == &other )
			return *this;
		continuations = other.continuations;
		reactive = other.reactive;
		status = other.status;
		return *this;
	}

	Property& operator=( Property&& other ) noexcept {
		if( this == &other )
			return *this;
		continuations = std::move( other.continuations );
		reactive = std::move( other.reactive );
		status = std::move( other.status );
		return *this;
	}


	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	friend std::ostream& operator<<( std::ostream& os, const PropertyView< T >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/**
 * \brief Python/C#-like syntatic sugar for class properties.
 * You define a class property like this:
 *  ```
 *     struct MyClass {
 *		MyClass() : name(this, std::mem_fn(&MyClass::getS), std::mem_fn(&MyClass::setS)) {}
 *     
 *		Accessor<std::string, MyClass> name;
 *		
 *		private:
 *			std::string s = '';
 *			
 *			std::string getS() { return s; }
 *			void setS(std::string new_s) { s = new_s; }
 *			
 *		}
 *		
 *	```
 * \tparam T Type
 * \tparam S Class name
 */
template< typename T, class S >
class Accessor {

	using get_fn = std::function< T( S& ) >;
	using set_fn = std::function< void( S&, T ) >;

public:

	Accessor() { }

	Accessor( S* cls, get_fn get, set_fn set ) : classptr( cls ),
	                                             getter( get ),
	                                             setter( set ) { }

	Accessor( S* cls, get_fn get ) : Accessor(cls, get, nullptr) { }

	Accessor( const Accessor& other )
		: classptr( other.classptr ),
		  getter( other.getter ),
		  setter( other.setter ) {}

	Accessor( Accessor&& other ) noexcept
		: classptr( other.classptr ),
		  getter( other.getter ),
		  setter( other.setter ) {}

	Accessor& operator=( const Accessor& other ) {
		if( this == &other )
			return *this;
		classptr = other.classptr;
		getter = other.getter;
		setter = other.setter;
		return *this;
	}

	Accessor& operator=( Accessor&& other ) noexcept {
		if( this == &other )
			return *this;
		classptr = other.classptr;
		getter = other.getter;
		setter = other.setter;
		return *this;
	}

	void operator=( T value ) const { if(setter) setter( *classptr, value ); } // maybe throw if nullptr?
	operator T() const { return getter( *classptr ); }

	T operator->() const { return get(); }

	T get() const { return getter( *classptr ); }

	// Boolean operators

	bool operator!() const { return !get(); }

	// Relational operators

	bool operator==( const T& rhs ) const { return ( rhs == get() ); }

	bool operator==( const Accessor< T, S >& rhs ) const { return ( rhs.get() == get() ); }

	bool operator!=( const T& rhs ) const { return !( rhs == get() ); }

	bool operator!=( const Accessor< T, S >& rhs ) const { return !( rhs.get() == get() ); }

	bool operator<( const T& rhs ) const { return ( rhs < get() ); }

	bool operator<( const Accessor< T, S >& rhs ) const { return ( rhs.get() < get() ); }

	bool operator>( const T& rhs ) const { return ( rhs > get() ); }

	bool operator>( const Accessor< T, S >& rhs ) const { return ( rhs.get() > get() ); }

	bool operator>=( const T& rhs ) const { return ( rhs >= get() ); }

	bool operator>=( const Accessor< T, S >& rhs ) const { return ( rhs.get() >= get() ); }

	bool operator<=( const T& rhs ) const { return ( rhs <= get() ); }

	bool operator<=( const Accessor< T, S >& rhs ) const { return ( rhs.get() <= get() ); }

private:

	mutable S* classptr = nullptr;

	mutable get_fn getter = nullptr;

	mutable set_fn setter = nullptr;

	T default_getter(S& ) {
		throw "Cannot read accessor";
	}

};


NAMESPACE_END

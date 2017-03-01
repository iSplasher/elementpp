#pragma once

#include "global.h"

#include <react/Domain.h>
#include <react/Signal.h>
#include <react/Event.h>

#include <type_traits>
#include <functional>
#include <initializer_list>
#include <list>

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
	* \brief Create a permanent connection that will be release when the connection object goes out of scope
	*/
	Scoped
};


/**
* \brief Create a property that acts like an event steam
*/
using PropertyEventType = struct PropertyEventType;

/**
 * \brief Create a read-only propertyevent that acts like a view for other propertyevents
 */
using PropertyEventViewType = struct PropertyEventViewType;

/**
* \brief Create a read-only property that acts like a view for other properties
*/
using PropertyViewType = struct PropertyViewType;


/**
 * \brief Private connection object returned when connecting property to functions
 * \tparam T Property<T>
 */
template< typename T > // TODO: static assert T is Property object
class Connection {
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;
public:

	Connection() {}

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
		if( cont && *cont ) {
			cont->reset();
		}
	};

	/**
	 * \brief Check if connection is linked to property
	 * \return 
	 */
	bool isReleased() const { return cont && *cont ? false : true; }; // TODO: maybe property?

private:

	Connection( T* t, react::TransactionStatus* s, std::unique_ptr< Continuation >* o, bool scope = false ) : scoped( scope ),
	                                                                                                          prop( t ),
	                                                                                                          status( s ),
	                                                                                                          cont( o ) {}

	bool scoped = false;
	T* prop = nullptr;
	react::TransactionStatus* status = nullptr;
	std::unique_ptr< Continuation >* cont = nullptr;

	friend T;
};


class Element;

template< typename T, typename AccessClass, typename AccessClass2 >
class Property; // Property<T, AccessClass>

template< typename T >
class Property< T, Element, void >; // Property<T>

template< typename T >
class Property< T, PropertyViewType, void >; // PropertyView<T>

template< typename T, typename AccessClass >
class Property< T, PropertyEventType, AccessClass >; // PropertyEvent<T, AccessClass>

template< typename T >
class Property< T, PropertyEventType, void >; // PropertyEvent<T>

template< typename T >
class Property< T, PropertyEventViewType, void >; // PropertyEventView<T>

template< typename T >
using PropertyView = Property< T, PropertyViewType, void >;

template< typename T, typename A = void >
using PropertyEvent = Property< T, PropertyEventType, A >;

template< typename T >
using PropertyEventView = Property< T, PropertyEventViewType, void >;


/**
 * \brief Private read-only property
 * \tparam T type
 * \tparam Private class allowed to access this property
 * \tparam A // unspecified //
 * \see PropertyView
 */
template< typename T, typename Private = Element, typename A = void >
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
		return Connection< Property< T, Private > >( this, &status, &cont, C == ConnectionType::Scoped );
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
	T get() { return reactive.Value(); }

	// Relational operators

	bool operator==( const T& rhs ) { return ( rhs == reactive.Value() ); }

	bool operator==( const Property< T, Private, A >& rhs ) { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) { return !( rhs == reactive.Value() ); }

	bool operator!=( const Property< T, Private, A >& rhs ) { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) { return ( rhs < reactive.Value() ); }

	bool operator<( const Property< T, Private, A >& rhs ) { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) { return ( rhs > reactive.Value() ); }

	bool operator>( const Property< T, Private, A >& rhs ) { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) { return ( rhs >= reactive.Value() ); }

	bool operator>=( const Property< T, Private, A >& rhs ) { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) { return ( rhs <= reactive.Value() ); }

	bool operator<=( const Property< T, Private, A >& rhs ) { return ( rhs.reactive.Value() <= reactive.Value() ); }

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

	Property( const Property& other ) = delete;

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
	* \brief Property = {value, value, ...}
	* \param value initializer list
	* \return
	*/
	auto& operator=(std::initializer_list< T > value) {
		for (auto& x : value) {
			this = x;
		}
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
	template< typename P, typename PropertyViewType, typename C >
	friend class Property;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Private, A >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/**
 * \brief A read-write property
 * \tparam T
 * \see PropertyView
 */
template< typename T >
class Property< T, Element, void > {
	using Reactive = PRIV_NAMESPACE::VarSignalT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	Property() : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T() ) ) { }

	template< typename ... Args >
	explicit Property( Args&& ... args ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( T( std::forward< Args >( args )... ) ) ) { }

	explicit Property( T&& t ) : reactive( react::MakeVar< PRIV_NAMESPACE::D >( std::forward< T >( t ) ) ) { }

	Property( const Property& other ) = delete;

	Property( Property&& other ) noexcept
		: continuations( std::move( other.continuations ) ),
		  reactive( std::move( other.reactive ) ),
		  status( std::move( other.status ) ) {}

	Property& operator=( const Property& other ) {
		reactive <<= other.reactive.Value();
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
	* \brief Property = {value, value, ...}
	* \param value initializer list
	* \return
	*/
	auto& operator=( std::initializer_list< T > value ) {
		for( auto& x : value ) {
			this = x;
		}
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
		return Connection< Property< T > >( this, &status, &cont, C == ConnectionType::Scoped );
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

	bool operator==( const Property< T, Element, void >& rhs ) const { return ( rhs.reactive.Value() == reactive.Value() ); }

	bool operator!=( const T& rhs ) const { return !( rhs == reactive.Value() ); }

	bool operator!=( const Property< T, Element, void >& rhs ) const { return !( rhs.reactive.Value() == reactive.Value() ); }

	bool operator<( const T& rhs ) const { return ( rhs < reactive.Value() ); }

	bool operator<( const Property< T, Element, void >& rhs ) const { return ( rhs.reactive.Value() < reactive.Value() ); }

	bool operator>( const T& rhs ) const { return ( rhs > reactive.Value() ); }

	bool operator>( const Property< T, Element, void >& rhs ) const { return ( rhs.reactive.Value() > reactive.Value() ); }

	bool operator>=( const T& rhs ) const { return ( rhs >= reactive.Value() ); }

	bool operator>=( const Property< T, Element, void >& rhs ) const { return ( rhs.reactive.Value() >= reactive.Value() ); }

	bool operator<=( const T& rhs ) const { return ( rhs <= reactive.Value() ); }

	bool operator<=( const Property< T, Element, void >& rhs ) const { return ( rhs.reactive.Value() <= reactive.Value() ); }

private:
	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	template< typename P, typename PropertyViewType, typename C >
	friend class Property;

	friend std::ostream& operator<<( std::ostream& os, const Property< T, Element, void >& obj ) {
		os << obj.reactive.Value();
		return os;
	}
};


/**
* \brief A property view
* \tparam T type
* \see Property
*/
template< typename T >
class Property< T, PropertyViewType, void > {
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
		return Connection< PropertyView< T > >( this, &status, &cont, C == ConnectionType::Scoped );
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
	T get() {
		return reactive.Value();;
	}

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
* \brief A read-only property event stream
* \tparam T
* \see PropertyEventView
*/
template< typename T, typename Private >
class Property< T, PropertyEventType, Private > {
	using Reactive = PRIV_NAMESPACE::EventSourceT< T >;
	using func = std::function< void(T) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	Property() : reactive(react::MakeEventSource< PRIV_NAMESPACE::D, T >()) { }

	~Property() {
		for (auto& p : continuations) {
			if (p) {
				p.reset();
			}
		}
	}

	// FUNCTIONS

	/**
	* \brief Construct a connection to a function that will be called on every property change
	* \tparam C connection release policy
	* \param f function to changed
	* \return
	*/
	template< ConnectionType C = ConnectionType::Permanent >
	auto changed(func f) {
		std::unique_ptr< Continuation > _cont = nullptr;
		continuations.push_back(std::move(_cont));
		auto& cont = continuations.back();

		switch (C) {
		case ConnectionType::Scoped:
		case ConnectionType::Permanent:
		{
			cont.reset(new Continuation(std::move(react::MakeContinuation(reactive, f))));
			break;
		}
		case ConnectionType::Temporary: // run only once
		{
			cont.reset(new Continuation(std::move(react::MakeContinuation(reactive, [=, &cont](T t) {
				f(std::forward< T >(t));
				if (cont)
					cont.reset();
			}))));
			break;
		}
		}
		return Connection< PropertyEvent< T, Private > >(this, &status, &cont, C == ConnectionType::Scoped);
	}

	/**
	* \brief Wait for all async computations have been made
	* \remark Equivalent to a thread.join()
	* \see operator<<()
	*/
	void wait() { status.Wait(); }

	// Relational operators

	bool operator==(const PropertyEvent< T, Private >& rhs) const { return rhs.reactive.Equals(reactive); }

	bool operator!=(const PropertyEvent< T, Private >& rhs) const { return !(rhs.reactive.Equals(reactive)); }

private:

	// MOVE & COPY

	Property(const Property& other)
		: continuations(other.continuations),
		reactive(other.reactive),
		status(other.status) {}

	Property(Property&& other) noexcept
		: continuations(std::move(other.continuations)),
		reactive(std::move(other.reactive)),
		status(std::move(other.status)) {}

	Property& operator=(const Property& other) {
		if (this == &other)
			return *this;
		continuations = other.continuations;
		reactive = other.reactive;
		status = other.status;
		return *this;
	}

	Property& operator=(Property&& other) noexcept {
		if (this == &other)
			return *this;
		continuations = std::move(other.continuations);
		reactive = std::move(other.reactive);
		status = std::move(other.status);
		return *this;
	}

	/**
	* \brief property = value
	* \param value T
	* \return
	*/
	auto& operator=(T value) {
		reactive << std::forward< T >(value);
		return *this;
	}

	/**
	* \brief Property = {value, value, ...}
	* \param value initializer list
	* \return
	*/
	auto& operator=(std::initializer_list< T > value) {
		for (auto& x : value) {
			this = x;
		}
		return *this;
	}

	/**
	* \brief Property << value. This operator will propogate the value asynchronously
	* \param value T
	* \see wait()
	*/
	void operator<<(T value) { react::AsyncTransaction< PRIV_NAMESPACE::D >(status, [=] { reactive << value; }); }

	/**
	* \brief Property << {value, value, ...}. This operator will propogate the value asynchronously
	* \param value initializer list
	* \see wait()
	*/
	void operator<<(std::initializer_list< T > value) {
		react::AsyncTransaction< PRIV_NAMESPACE::D >(status, [=] {
			for (auto& x : value) {
				reactive << x;
			}
		});
	}

	/**
	* \brief Property( value )
	* \param value T
	* \param async propogate this value asynchronously
	*/
	void operator()(T value, bool async = false) {
		if (async)
			*this << std::forward< T >(value);
		else
			*this = std::forward< T >(value);
	}

	/**
	* \brief Property( {value, value, ...} )
	* \param value initializer list
	*/
	void operator()(std::initializer_list< T > value) {
		this << std::forward< std::initializer_list< T > >(value);
	}

	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	friend Private;

	template< typename P, typename PropertyEventViewType, typename C >
	friend class Property;
};


/**
* \brief A read-write property event stream
* \tparam T
* \see PropertyEventView
*/
template< typename T >
class Property< T, PropertyEventType, void > {
	using Reactive = PRIV_NAMESPACE::EventSourceT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	Property() : reactive( react::MakeEventSource< PRIV_NAMESPACE::D, T >() ) { }

	Property( const Property& other ) = delete;

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
		reactive << std::forward< T >( value );
		return *this;
	}

	/**
	 * \brief Property = {value, value, ...}
	 * \param value initializer list
	 * \return 
	 */
	auto& operator=( std::initializer_list< T > value ) {
		for( auto& x : value ) {
			this = x;
		}
		return *this;
	}

	/**
	* \brief Property << value. This operator will propogate the value asynchronously
	* \param value T
	* \see wait()
	*/
	void operator<<( T value ) { react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [=] { reactive << value; } ); }

	/**
	* \brief Property << {value, value, ...}. This operator will propogate the value asynchronously
	* \param value initializer list
	* \see wait()
	*/
	void operator<<( std::initializer_list< T > value ) {
		react::AsyncTransaction< PRIV_NAMESPACE::D >( status, [=] {
			                                              for( auto& x : value ) {
				                                              reactive << x;
			                                              }
		                                              } );
	}

	/**
	 * \brief Property( value )
	 * \param value T
	 * \param async propogate this value asynchronously
	 */
	void operator()( T value, bool async = false ) {
		if( async )
			*this << std::forward< T >( value );
		else
			*this = std::forward< T >( value );
	}

	/**
	 * \brief Property( {value, value, ...} )
	 * \param value initializer list
	 */
	void operator()( std::initializer_list< T > value ) {
		this << std::forward< std::initializer_list< T > >( value );
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
		return Connection< PropertyEvent< T > >( this, &status, &cont, C == ConnectionType::Scoped );
	}

	/**
	* \brief Wait for all async computations have been made
	* \remark Equivalent to a thread.join()
	* \see operator<<()
	*/
	void wait() { status.Wait(); }

	// Relational operators

	bool operator==( const PropertyEvent< T >& rhs ) const { return rhs.reactive.Equals( reactive ); }

	bool operator!=( const PropertyEvent< T >& rhs ) const { return !( rhs.reactive.Equals( reactive ) ); }

private:
	std::list< std::unique_ptr< Continuation > > continuations;
	Reactive reactive;
	react::TransactionStatus status = react::TransactionStatus();

	template< typename P, typename PropertyEventViewType, typename C >
	friend class Property;
};


/**
* \brief A propertyevent view
* \tparam T type
* \see PropertyEvent
*/
template< typename T >
class Property< T, PropertyEventViewType, void > {
	using Reactive = PRIV_NAMESPACE::EventsT< T >;
	using func = std::function< void( T ) >;
	using Continuation = react::Continuation< PRIV_NAMESPACE::D >;

public:

	template< typename... Args >
	explicit Property( Args&&... args ) {
		// TODO: find a way to static_assert function arguments count
		reactive = react::Merge( args.reactive ... );
	}

	~Property() {
		for( auto& p : continuations ) {
			if( p ) {
				p.reset();
			}
		}
	}

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
		return Connection< PropertyEventView< T > >( this, &status, &cont, C == ConnectionType::Scoped );
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
	T get() {
		return reactive.Value();;
	}

	// Relational operators

	bool operator==( const PropertyEventView< T >& rhs ) { return rhs.reactive.Equals( reactive ); }

	bool operator!=( const PropertyEventView< T >& rhs ) { return !( rhs.reactive.Equals( reactive ) ); }

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

	Accessor( S* cls, get_fn get ) : Accessor( cls, get, nullptr ) { }

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

	void operator=( T value ) const {
		if( setter )
			setter( *classptr, value );
	} // maybe throw if nullptr?
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

	T default_getter( S& ) {
		throw "Cannot read accessor";
	}

};


NAMESPACE_END

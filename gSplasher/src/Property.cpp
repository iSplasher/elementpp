#include "gSplasher/Property.h"

USING_NAMESPACE

template< typename T, ConnectionReleasePolicy R >
Connection< T, R >& Connection< T, R >::operator=( const Connection& other ) {
	if( this == &other )
		return *this;
	prop = other.prop;
	status = other.status;
	obs = other.obs;
	released = other.released;
	return *this;
}

template< typename T, ConnectionReleasePolicy R >
Connection< T, R >& Connection< T, R >::operator=( Connection&& other ) noexcept {
	if( this == &other )
		return *this;
	prop = other.prop;
	status = other.status;
	obs = std::move( other.obs );
	released = other.released;
	return *this;
}

template< typename T, ConnectionReleasePolicy R >
Connection< T, R >::Connection( T& t, PRIV_NAMESPACE::ObserverT o, react::TransactionStatus& s ) : prop( t ),
                                                                                                   status( s ),
                                                                                                   obs( o ) {}

template< typename T, ConnectionReleasePolicy R >
Connection< T, R >::~Connection() { if( R == Immediate ) { release(); } }

template< typename T, ConnectionReleasePolicy R >
bool Connection< T, R >::release() {
	if( obs.IsValid() )
		obs.Detach();
	return !obs.IsValid();
}

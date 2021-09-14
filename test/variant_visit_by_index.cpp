// Copyright 2017, 2021 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>

using namespace boost::variant2;

int main()
{
    {
        variant<int, int, float> v;

        visit_by_index( v,
            []( int& x ){ BOOST_TEST_EQ( x, 0 ); },
            []( int&   ){ BOOST_ERROR( "incorrect alternative" ); },
            []( float& ){ BOOST_ERROR( "incorrect alternative" ); } );
    }

    {
        variant<int const, int, float const> v( 1 );

        visit_by_index( v,
            []( int const& x ){ BOOST_TEST_EQ( x, 1 ); },
            []( int&         ){ BOOST_ERROR( "incorrect alternative" ); },
            []( float const& ){ BOOST_ERROR( "incorrect alternative" ); } );
    }

    {
        variant<int, int, float> const v( in_place_index_t<1>(), 2 );

        visit_by_index( v,
            []( int const&   ){ BOOST_ERROR( "incorrect alternative" ); },
            []( int const& x ){ BOOST_TEST_EQ( x, 2 ); },
            []( float const& ){ BOOST_ERROR( "incorrect alternative" ); } );
    }

    {
        variant<int const, int, float const> const v( 3.14f );

        visit_by_index( v,
            []( int const&     ){ BOOST_ERROR( "incorrect alternative" ); },
            []( int const&     ){ BOOST_ERROR( "incorrect alternative" ); },
            []( float const& x ){ BOOST_TEST_EQ( x, 3.14f ); } );
    }

    return boost::report_errors();
}

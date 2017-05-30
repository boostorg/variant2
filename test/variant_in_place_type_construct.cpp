
// Copyright 2017 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>
#include <utility>
#include <string>

using namespace boost::variant2;

struct X
{
    X() = default;
    template<class T> X( in_place_type_t<T> ) = delete;
};

int main()
{
    {
        variant<int> v( in_place_type<int> );

        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 0 );

        BOOST_TEST( holds_alternative<int>(v) );
    }

    {
        variant<X> v( in_place_type<X> );

        BOOST_TEST_EQ( v.index(), 0 );

        BOOST_TEST( holds_alternative<X>(v) );
    }

    {
        variant<int> v( in_place_type<int>, 1 );

        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 1 );

        BOOST_TEST( holds_alternative<int>(v) );
    }

    {
        variant<int, float> v( in_place_type<int> );

        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 0 );

        BOOST_TEST( holds_alternative<int>(v) );
    }

    {
        variant<int, float> v( in_place_type<int>, 1 );

        BOOST_TEST_EQ( v.index(), 0 );
        BOOST_TEST_EQ( get<0>(v), 1 );

        BOOST_TEST( holds_alternative<int>(v) );
    }

    {
        variant<int, float> v( in_place_type<float> );

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 0 );

        BOOST_TEST( holds_alternative<float>(v) );
    }

    {
        variant<int, float> v( in_place_type<float>, 3.14f );

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), 3.14f );

        BOOST_TEST( holds_alternative<float>(v) );
    }

    {
        variant<int, int, float, std::string> v( in_place_type<float>, 3.14f );

        BOOST_TEST_EQ( v.index(), 2 );
        BOOST_TEST_EQ( get<2>(v), 3.14f );

        BOOST_TEST( holds_alternative<float>(v) );
    }

    {
        variant<int, int, float, float, std::string> v( in_place_type<std::string>, "text" );

        BOOST_TEST_EQ( v.index(), 4 );
        BOOST_TEST_EQ( get<4>(v), std::string("text") );

        BOOST_TEST( holds_alternative<std::string>(v) );
    }

    {
        variant<int, int, float, float, std::string> v( in_place_type<std::string>, 4, 'a' );

        BOOST_TEST_EQ( v.index(), 4 );
        BOOST_TEST_EQ( get<4>(v), std::string( 4, 'a' ) );

        BOOST_TEST( holds_alternative<std::string>(v) );
    }

    return boost::report_errors();
}

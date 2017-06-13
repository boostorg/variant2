
// Copyright 2017 Peter Dimov.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <type_traits>
#include <utility>
#include <string>
#include <stdexcept>

using namespace boost::variant2;

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

enum E1
{
    e1
};

enum E2
{
    e2
};

struct X1
{
    X1() = default;
    X1( E2 ) { throw std::runtime_error( "X1(E2)" ); }
};

STATIC_ASSERT( std::is_nothrow_copy_constructible<X1>::value );
STATIC_ASSERT( std::is_nothrow_move_constructible<X1>::value );
STATIC_ASSERT( !std::is_nothrow_constructible<X1, E2>::value );
STATIC_ASSERT( std::is_trivially_destructible<X1>::value );

struct X2
{
    X2() = default;
    ~X2();
    X2( E2 ) { throw std::runtime_error( "X1(E2)" ); }
};

X2::~X2() {}

STATIC_ASSERT( std::is_nothrow_copy_constructible<X2>::value );
STATIC_ASSERT( std::is_nothrow_move_constructible<X2>::value );
STATIC_ASSERT( !std::is_nothrow_constructible<X2, E2>::value );
STATIC_ASSERT( !std::is_trivially_destructible<X2>::value );

struct X3
{
    X3() = default;
    X3( X3 const& ) {}
    X3( X3&& ) {}
};

STATIC_ASSERT( !std::is_nothrow_copy_constructible<X3>::value );
STATIC_ASSERT( !std::is_nothrow_move_constructible<X3>::value );
STATIC_ASSERT( std::is_trivially_destructible<X3>::value );

int main()
{
    {
        variant<monostate, E1, X1> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    {
        variant<X3, E1, X1> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    {
        variant<valueless, E1, X1> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            // all types are trivial, so we get a constexpr path
            // and v.index() stays 1

            BOOST_TEST( v.index() == 0 || v.index() == 1 );

            if( v.index() == 1 )
            {
                BOOST_TEST_EQ( get<1>(v), e1 );
            }
        }
    }

    {
        variant<monostate, E1, X1, valueless> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    {
        variant<monostate, E1, X2> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    {
        variant<X3, E1, X2> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    {
        variant<valueless, E1, X2> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 0 );
        }
    }

    {
        variant<monostate, E1, X2, valueless> v;

        v = e1;

        BOOST_TEST_EQ( v.index(), 1 );
        BOOST_TEST_EQ( get<1>(v), e1 );

        try
        {
            v = e2;
            BOOST_ERROR( "`v = e2;` failed to throw" );
        }
        catch( std::exception const& )
        {
            BOOST_TEST_EQ( v.index(), 1 );
            BOOST_TEST_EQ( get<1>(v), e1 );
        }
    }

    return boost::report_errors();
}

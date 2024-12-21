// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if defined(_MSC_VER)
# pragma warning( disable: 4503 ) // decorated name length exceeded
#endif

#include <boost/variant2/variant.hpp>
#include <boost/mp11.hpp>
#include <boost/core/lightweight_test.hpp>

using namespace boost::mp11;

template<std::size_t I> struct X
{
};

template<class N> using Xf = X< N::value >;

template<std::size_t I> struct Y
{
    Y() = default;
    Y( Y&& ) {}
};

template<class N> using Yf = Y< N::value >;

template<std::size_t I> struct Z
{
    ~Z() {}
};

template<class N> using Zf = Z< N::value >;

template<std::size_t I> struct W
{
    ~W() {}

    W() = default;
    W( W&& ) {}
};

template<class N> using Wf = W< N::value >;

template<template<class...> class F, std::size_t N> void test_()
{
    using V = mp_rename< mp_transform<F, mp_iota_c<N>>, boost::variant2::variant >;

    {
        V v( mp_back<V>{} );
        BOOST_TEST_EQ( v.index(), mp_size<V>() - 1 );
    }

    {
        V v;
        BOOST_TEST_EQ( v.index(), 0 );

        v.template emplace< mp_back<V> >();
        BOOST_TEST_EQ( v.index(), mp_size<V>() - 1 );
    }
}

template<template<class...> class F> void test()
{
    test_<F, 126>();
    test_<F, 127>();
    test_<F, 128>();
    test_<F, 129>();

    test_<F, 254>();
    test_<F, 255>();
    test_<F, 256>();
    test_<F, 257>();
}

int main()
{
    static_assert( !boost::variant2::variant< X<0>, X<1> >::uses_double_storage(), "" );
    test<Xf>();

    static_assert( boost::variant2::variant< Y<0>, Y<1> >::uses_double_storage(), "" );
    test<Yf>();

    static_assert( !boost::variant2::variant< Z<0>, Z<1> >::uses_double_storage(), "" );
    test<Zf>();

    static_assert( boost::variant2::variant< W<0>, W<1> >::uses_double_storage(), "" );
    test<Wf>();

    return boost::report_errors();
}

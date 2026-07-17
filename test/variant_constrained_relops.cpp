
// Copyright 2026 Joaquin M Lopez Munoz.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>

using namespace boost::variant2;

struct not_found {};
struct any { any( ... ); };
not_found operator==( any const &, any const & );
not_found operator!=( any const &, any const & );
not_found operator< ( any const &, any const & );
not_found operator<=( any const &, any const & );
not_found operator> ( any const &, any const & );
not_found operator>=( any const &, any const & );

struct no_eq
{
    int  operator!=( const no_eq& ) const;
    int  operator< ( const no_eq& ) const;
    int  operator<=( const no_eq& ) const;
};

struct bad_eq
{
    void operator==( const bad_eq& ) const;
    int  operator!=( const bad_eq& ) const;
    int  operator< ( const bad_eq& ) const;
    int  operator<=( const bad_eq& ) const;
};

struct no_neq
{
    int  operator==( const no_neq& ) const;
    int  operator< ( const no_neq& ) const;
    int  operator<=( const no_neq& ) const;
};

struct bad_neq
{
    int  operator==( const bad_neq& ) const;
    void operator!=( const bad_neq& ) const;
    int  operator< ( const bad_neq& ) const;
    int  operator<=( const bad_neq& ) const;
};

struct no_lt
{
    int  operator==( const no_lt& ) const;
    int  operator!=( const no_lt& ) const;
    int  operator<=( const no_lt& ) const;
};

struct bad_lt
{
    int  operator==( const bad_lt& ) const;
    int  operator!=( const bad_lt& ) const;
    void operator< ( const bad_lt& ) const;
    int  operator<=( const bad_lt& ) const;
};

struct no_le
{
    int  operator==( const no_le& ) const;
    int  operator!=( const no_le& ) const;
    int  operator< ( const no_le& ) const;
};

struct bad_le
{
    int  operator==( const bad_le& ) const;
    int  operator!=( const bad_le& ) const;
    int  operator< ( const bad_le& ) const;
    void operator<=( const bad_le& ) const;
};

int main()
{
    {
        using v_t = const variant<int, no_eq>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, bad_eq>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, no_neq>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, bad_neq>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, no_lt>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, bad_lt>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, no_le>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), not_found>));
    }

    {
        using v_t = const variant<int, bad_le>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() != std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), not_found>));
    }

    return boost::report_errors();
}

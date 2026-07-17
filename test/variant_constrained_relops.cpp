
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

#if !defined(BOOST_NO_SFINAE_EXPR) && \
    !BOOST_WORKAROUND(BOOST_MSVC, < 1900) && !BOOST_WORKAROUND(BOOST_GCC, < 40900)
#define USE_DELETED_RELOPS
#endif

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
#ifdef USE_DELETED_RELOPS
    bool operator==( const no_eq& ) const = delete;
#endif
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

struct no_ne
{
    int  operator==( const no_ne& ) const;
#ifdef USE_DELETED_RELOPS
    bool operator!=( const no_ne& ) const = delete;
#endif
    int  operator< ( const no_ne& ) const;
    int  operator<=( const no_ne& ) const;
};

struct bad_ne
{
    int  operator==( const bad_ne& ) const;
    void operator!=( const bad_ne& ) const;
    int  operator< ( const bad_ne& ) const;
    int  operator<=( const bad_ne& ) const;
};

struct no_lt
{
    int  operator==( const no_lt& ) const;
    int  operator!=( const no_lt& ) const;
#ifdef USE_DELETED_RELOPS
    bool operator< ( const no_lt& ) const = delete;
#endif
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
#ifdef USE_DELETED_RELOPS
    bool operator<=( const no_le& ) const = delete;
#endif
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
        using v_t = const variant<int, no_ne>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( operator!=( std::declval<v_t>(), std::declval<v_t>() ) ), not_found>));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() <= std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >  std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() >= std::declval<v_t>() ), bool     >));
    }

    {
        using v_t = const variant<int, bad_ne>&;

        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( std::declval<v_t>() == std::declval<v_t>() ), bool     >));
        BOOST_TEST_TRAIT_TRUE((std::is_same<decltype( operator!= ( std::declval<v_t>(), std::declval<v_t>() ) ), not_found>));
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

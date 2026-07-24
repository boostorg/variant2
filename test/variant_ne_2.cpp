// Copyright 2026 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/mp11.hpp>
#include <type_traits>

template<class T> using has_ne_impl = decltype( std::declval<T const&>() != std::declval<T const&>() );
template<class T> using has_ne = boost::mp11::mp_valid<has_ne_impl, T>;

using namespace boost::variant2;

struct X1
{
};

struct X2
{
    bool operator!=( X2 const& ) const = delete;
};

struct X3
{
    void operator!=( X3 const& ) const;
};

int main()
{
    BOOST_TEST_TRAIT_FALSE(( has_ne< variant<X1> > ));
    BOOST_TEST_TRAIT_FALSE(( has_ne< variant<X2> > ));
    BOOST_TEST_TRAIT_FALSE(( has_ne< variant<X3> > ));

    return boost::report_errors();
}

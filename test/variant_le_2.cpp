// Copyright 2026 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1920

BOOST_PRAGMA_MESSAGE( "Test skipped, because BOOST_MSVC < 1920" )
int main() {}

#else

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/mp11.hpp>

using boost::variant2::detail::has_le;

using namespace boost::variant2;

struct X1
{
};

struct X2
{
    bool operator<=( X2 const& ) const = delete;
};

struct X3
{
    void operator<=( X3 const& ) const;
};

struct X4
{
    bool operator<=( X4 const& ) const;
};

int main()
{
    BOOST_TEST_TRAIT_FALSE(( has_le< X1 > ));
    BOOST_TEST_TRAIT_FALSE(( has_le< X2 > ));
    BOOST_TEST_TRAIT_FALSE(( has_le< X3 > ));
    BOOST_TEST_TRAIT_TRUE(( has_le< X4 > ));

    BOOST_TEST_TRAIT_FALSE(( has_le< variant<X1> > ));
    BOOST_TEST_TRAIT_FALSE(( has_le< variant<X2> > ));
    BOOST_TEST_TRAIT_FALSE(( has_le< variant<X3> > ));
    BOOST_TEST_TRAIT_TRUE(( has_le< variant<X4> > ));

    return boost::report_errors();
}

#endif

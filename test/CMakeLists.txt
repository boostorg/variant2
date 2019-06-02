# Copyright 2018 Peter Dimov
# Copyright 2019 Hans Dembinski
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

boost_fetch(boostorg/assert TAG develop)
boost_fetch(boostorg/core TAG develop)
boost_fetch(boostorg/preprocessor TAG develop)
boost_fetch(boostorg/type_traits TAG develop)
boost_fetch(boostorg/static_assert TAG develop)
boost_fetch(boostorg/mp11 TAG develop)

boost_test_jamfile(FILE Jamfile LIBRARIES Boost::variant2 Boost::core Boost::detail Boost::assert Boost::preprocessor Boost::type_traits Boost::static_assert Boost::mp11)

# variant2

This repository contains a never-valueless C++14 implementation of [std::variant](http://en.cppreference.com/w/cpp/utility/variant) in [variant.hpp](include/boost/variant2/variant.hpp) and an implementation of `expected<T, E...>` in [expected.hpp](include/boost/variant2/variant.hpp) that is an extended version of `extended<T, E>` as proposed in [P0323R1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf).

The code requires [mp11](https://github.com/pdimov/mp11) and Boost.Config. The repository is intended to be placed into the `libs/variant2` directory of a Boost clone or release, with mp11 in `libs/mp11`, but the headers will also work standalone if [mp11.hpp](https://github.com/pdimov/mp11/blob/master/include/boost/mp11.hpp) or [mp11_single.hpp](https://github.com/pdimov/mp11/blob/master/include/boost/mp11_single.hpp) is included beforehand.

Supported compilers:

* g++ 5 or later with -std=c++14 or -std=c++1z
* clang++ 3.5 or later with -std=c++14 or -std=c++1z
* Visual Studio 2017

## variant.hpp

The class `boost::variant2::variant<T...>` is an almost conforming implementation of `std::variant` with the following differences:

* The function `valueless_by_exception()` is not present, as the variant is never valueless;
* A converting constructor from, e.g. `variant<int, float>` to `variant<float, double, int>` is provided as an extension;
* The reverse operation, going from `variant<float, double, int>` to `variant<int, float>` is provided as the member function `subset<U...>`. (This operation can throw if the current state of the variant cannot be represented.)

To avoid the valueless state, this implementation falls back to using double storage unless

* all the contained types are nothrow move constructible, or
* at least one contained type has a nothrow default constructor.

If the second bullet doesn't hold, but the first does, the variant uses single storage, but `emplace` constructs a temporary and moves it into place if the construction of the object can throw. In case this is undesirable, the recommended practice is to add an alternative that has a nonthrowing default constructor.

## expected.hpp

The class `boost::variant2::expected<T, E...>` represents the return type of an operation that may potentially fail. It contains either the expected result of type `T`, or a reason for the failure of one of the error types in `E...`. Internally, this is represented as `variant<T, E...>`.

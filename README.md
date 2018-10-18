# variant2

This repository contains a never-valueless C++11/14/17 implementation of
[std::variant](http://en.cppreference.com/w/cpp/utility/variant) in
[variant.hpp](include/boost/variant2/variant.hpp) and an implementation of
`expected<T, E...>` in [expected.hpp](include/boost/variant2/expected.hpp)
that is an extended version of `expected<T, E>` as proposed in
[P0323R1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf)
and the subsequent
[D0323R2](https://github.com/viboes/std-make/blob/master/doc/proposal/expected/d0323r2.md).

The code requires [Boost.Mp11](https://github.com/boostorg/mp11) and
Boost.Config.

The repository is intended to be placed into the `libs/variant2` directory of
a Boost clone or release, but the header `variant.hpp` will also work
[standalone](https://godbolt.org/z/6LSV3Z).

Supported compilers:

* g++ 4.8 or later with -std=c++11 or above
* clang++ 3.5 or later with -std=c++11 or above
* Visual Studio 2015, 2017

Tested on [Travis](https://travis-ci.org/pdimov/variant2/) and
[Appveyor](https://ci.appveyor.com/project/pdimov/variant2/).

## variant.hpp

The class `boost::variant2::variant<T...>` is an almost conforming
implementation of `std::variant` with the following differences:

* A converting constructor from, e.g. `variant<int, float>` to
  `variant<float, double, int>` is provided as an extension;
* The reverse operation, going from `variant<float, double, int>` to
  `variant<int, float>` is provided as the member function `subset<U...>`.
  (This operation can throw if the current state of the variant cannot be
  represented.)
* `variant<T...>` is not trivial when all contained types are trivial.

To avoid going into a valueless-by-exception state, this implementation falls
back to using double storage unless

* one of the alternatives is the type `monostate`,
* one of the alterantives has a nonthtowing default constructor, or
* all the contained types are nothrow move constructible.

If the first two bullets doesn't hold, but the third does, the variant uses
single storage, but `emplace` constructs a temporary and moves it into place
if the construction of the object can throw. In case this is undesirable, one
can force `emplace` into always constructing in-place by adding `monostate` as
one of the alternatives.

## expected.hpp

The class `boost::variant2::expected<T, E...>` represents the return type of
an operation that may potentially fail. It contains either the expected result
of type `T`, or a reason for the failure, of one of the error types in `E...`.
Internally, this is stored as `variant<T, E...>`.

See [its documentation](doc/expected.md) for more information.

Note that, while `variant` is production quality, `expected` is still a work
in progress and has no test suite yet.

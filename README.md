# variant2

A never-valueless C++14 implementation of [std::variant](http://en.cppreference.com/w/cpp/utility/variant). Requires [mp11](https://github.com/pdimov/mp11) and Boost.Config. Intended to be placed into the `libs/variant2` directory of a Boost clone or release, with mp11 in `libs/mp11`.

To avoid the valueless state, this implementation falls back to using double storage unless

* all the contained types are nothrow move constructible, or
* at least one contained type has a nothrow default constructor.

Supported compilers:

* g++ 5 or later with -std=c++14 or -std=c++1z
* clang++ 3.5 or later with -std=c++14 or -std=c++1z
* Visual Studio 2017

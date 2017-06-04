# expected<T, E...>

## Description

The class `expected<T, E...>` presented here is an extended version of `expected<T, E>` as
proposed in [P0323R1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf)
and the subsequent [D0323R2](https://github.com/viboes/std-make/blob/master/doc/proposal/expected/d0323r2.md).

The main difference is that this class takes more than one error type, which makes it more
flexible. One example of a type of the `expected` family, [`outcome<T>`](https://ned14.github.io/boost.outcome/),
on failure can store either an error of type `std::error_code`, or an exception in the form of `std::exception_ptr`.
This can be represented naturally in this implementation via `expected<T, std::error_code, std::exception_ptr>`.

In addition, libraries would generally differ in their choice of error types. It would be a
common need in practice of having to combine the results of calling two different libraries,
each with its own error type. Library 1 may use `lib1::error`:

    namespace lib1
    {

    enum class error
    {
        division_by_zero,
        other_error
    };

    expected<double, error> div( double x, double y );

    } // namespace lib1

while Library 2 might define its own `lib2::error`:

    namespace lib2
    {

    enum class error
    {
        division_by_zero,
        negative_logarithm
    };

    expected<double, error> log( double x );

    } // namespace lib2

In this proposal, combining the results of `lib1::div` and `lib2::log` can be achieved via
simple composition:

    expected<double, lib1::error, lib2::error> log_div_mul( double x, double y, double m )
    {
        auto r1 = lib1::div( x, y );
        if( !r1 ) return r1.unexpected();

        auto r2 = lib2::log( r1.value() );
        if( !r2 ) return r2.unexpected();

        return m * r2.value();
    }

An alternative approach that requires more effort is also supported:

    enum class common_error
    {
        division_by_zero,
        negative_logarithm,
        other_error,
        unknown_error
    };

    common_error make_common_error( lib1::error e );
    common_error make_common_error( lib2::error e );

    expected<double, common_error> log_div_mul2( double x, double y, double m )
    {
        static const auto rm = []( auto x ) { return make_common_error(x); };

        auto r1 = lib1::div( x, y ).remap_errors( rm );
        if( !r1 ) return r1.unexpected();

        auto r2 = lib2::log( r1.value() ).remap_errors( rm );
        if( !r2 ) return r2.unexpected();

        return m * r2.value();
    }

`std::error_code` is a very good choice for a common error type, and it's supported
natively by the overload of `.remap_errors()` that takes no arguments, which uses
calls to `make_error_code` to translate the errors.

When an attempt to access the value via `r.value()` is made and an error is present,
an exception is thrown. By default, this exception is of type `bad_expected_access<E>`,
as in D0323R2, but there are two differences. First, `bad_expected_access<E>` objects
derive from a common base `bad_expected_access<void>` so that they can be caught at
points where the set of possible `E` is unknown.

Second, the thrown exception can be customized. The implementation calls
`throw_on_unexpected(e)` unqualified, where `e` is the error object, and the user can
define such a function in the namespace of the type of `e`. Two specialized overloads
of `throw_on_unexpected` are provided, one for `std::error_code`, which throws the
corresponding `std::system_error`, and one for `std::exception_ptr`, which rethrows
the exception stored in it.

For example, `lib1` from above may customize the exceptions associated with `lib1::error`
via the following:

    namespace lib1
    {

    enum class error
    {
        division_by_zero,
        other_error
    };

    class exception: public std::exception
    {
    private:

        error e_;

    public:

        explicit exception( error e ): e_( e ) {}
        virtual const char * what() const noexcept;
    };

    void throw_on_unexpected( error e )
    {
        throw exception( e );
    }

    } // namespace lib1

In this implementation, `unexpected_type<E...>` has been called `unexpected_<T...>` and is
an alias for `variant<T...>`. It is unfortunately not possible to use the name `unexpected<T...>`,
because a function `std::unexpected` already exists.

The `make_...` helper functions have been omitted as unnecessary; class template argument deduction
as in `expected{ 1.0 }` or `unexpected_{ lib1::division_by_zero }` suffices.

Other functions have also been dropped as unnecessary, not providing sufficient value, dangerous, or
a combination of the three, although the decision of what to include isn't final at this point. The aim
is to produce a minimal interface that still covers the use cases.

`expected<T, E1...>` can be converted to `expected<T, E2...>` if all error types in `E1...` are
also in `E2...`. This allows composition as in the example above. Whether value convertibility ought
to also be supported is an open question.

A single monadic operation ("bind") is supported in the form of `operator>>`, allowing

    auto log_div_mul3( double x, double y, double m )
    {
        return lib1::div( x, y ) >> [&]( auto && r1 ) {

            return lib2::log( r1 ) >> [&]( auto && r2 ) -> expected<double, lib1::error, lib2::error> {

                return m * r2;

            };
        };
    }

as well as the more concise in this example, although limited in utility for real world scenarios,

    auto log_div_mul3( double x, double y, double m )
    {
        return lib1::div( x, y ) >> std::bind<expected<double, lib1::error, lib2::error>>( lib2::log, _1 ) >> m * _1;
    }

The more traditional name `then` was also a candidate for this operation, but `operator>>` has two advantages;
it avoids the inevitable naming debates and does not require parentheses around the continuation lambda.

## Synopsis

    // unexpected_

    template<class... E> using unexpected_ = variant<E...>;

    // bad_expected_access

    template<class E = void> class bad_expected_access;

    template<> class bad_expected_access<void>: public std::exception
    {
    public:

        bad_expected_access() noexcept;
        char const * what() const noexcept;
    };

    template<class E> class bad_expected_access: public bad_expected_access<void>
    {
    public:

        explicit bad_expected_access( E const& e );
        E error() const;
    };

    // throw_on_unexpected

    template<class E> void throw_on_unexpected( E const& e );
    void throw_on_unexpected( std::error_code const& e );
    void throw_on_unexpected( std::exception_ptr const& e );

    // expected

    template<class T, class... E> class expected
    {
    public:

        // value constructors

        constexpr expected() noexcept( /*see below*/ );

        constexpr expected( T const& t ) noexcept( /*see below*/ );
        constexpr expected( T&& t ) noexcept( /*see below*/ );

        // unexpected constructor

        template<class... E2>
        constexpr expected( unexpected_<E2...> const& x );

        template<class... E2>
        constexpr expected( unexpected_<E2...>&& x );

        // conversion constructor

        template<class... E2>
        constexpr expected( expected<T, E2...> const& x );

        template<class... E2>
        constexpr expected( expected<T, E2...>&& x );

        // emplace

        template<class... A> void emplace( A&&... a );
        template<class V, class... A> void emplace( std::initializer_list<V> il, A&&... a );

        // swap

        void swap( expected& r ) noexcept( /*see below*/ );

        // value queries

        constexpr bool has_value() const noexcept;
        constexpr explicit operator bool() const noexcept;

        // checked value access

        constexpr T& value() &;
        constexpr T const& value() const&;
        constexpr T&& value() &&;
        constexpr T const&& value() const&&;

        // unchecked value access

        T* operator->() noexcept;
        T const* operator->() const noexcept;

        T& operator*() & noexcept;
        T const& operator*() const & noexcept;
        T&& operator*() && noexcept;
        T const&& operator*() const && noexcept;

        // error queries

        template<class E2> constexpr bool has_error() const noexcept;
        constexpr bool has_error() const noexcept;

        // error access

        unexpected_<E...> unexpected() const;

        template<class E2> constexpr E2 error() const noexcept;
        constexpr /*see below*/ error() const noexcept;

        // error mapping

        template<class F> /*see below*/ remap_errors( F&& f ) const;
        expected<T, std::error_code> remap_errors() const;

        // then

        template<class F> /*see below*/ operator>>( F&& f ) const;
    };

    template<class T, class... E>
    inline constexpr bool operator==( expected<T, E...> const& x1, expected<T, E...> const& x2 );
    
    template<class T, class... E>
    inline constexpr bool operator!=( expected<T, E...> const& x1, expected<T, E...> const& x2 );

    template<class T, class... E>
    inline void swap( expected<T, E...>& x1, expected<T, E...>& x2 ) noexcept( /*see below*/ );

    // is_expected

    template<class T> struct is_expected;

    } // namespace variant2
    } // namespace boost

## Reference

...

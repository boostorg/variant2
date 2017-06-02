#ifndef BOOST_VARIANT2_EXPECTED_HPP_INCLUDED
#define BOOST_VARIANT2_EXPECTED_HPP_INCLUDED

//  Copyright 2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VARIANT2_VARIANT_HPP_INCLUDED
#include <boost/variant2/variant.hpp>
#endif
#include <boost/core/demangle.hpp>
#include <system_error>
#include <type_traits>
#include <typeinfo>
#include <cassert>

//

namespace boost
{
namespace variant2
{

// unexpected_

template<class... E> using unexpected_ = variant<E...>;

// bad_expected_access

template<class E = void> class bad_expected_access;

template<> class bad_expected_access<void>: public std::exception
{
private:

    std::string msg_;

public:

    bad_expected_access() noexcept
    {
    }

    explicit bad_expected_access( std::string&& msg ) noexcept: msg_( std::move(msg) ) // extension
    {
    }

    char const * what() const noexcept
    {
        return msg_.empty()? "bad_expected_access<>": msg_.c_str();
    }
};

namespace detail
{

template<class E, class En = std::enable_if_t<!std::is_enum<E>::value>> std::string add_value( E const& /*e*/ )
{
    return std::string();
}

template<class E, class E1 = void, class E2 = std::enable_if_t<std::is_enum<E>::value>> std::string add_value( E const& e )
{
    return ": " + std::to_string( static_cast<int>(e) );
}

} // namespace detail

template<class E> class bad_expected_access: public bad_expected_access<void>
{
private:

    E e_;

public:

    explicit bad_expected_access( E const& e )
        noexcept( std::is_nothrow_copy_constructible<E>::value )
        : bad_expected_access<void>( "bad_expected_access<" + boost::core::demangle( typeid(E).name() ) + ">" + variant2::detail::add_value( e ) ), e_( e )
    {
    }

    explicit bad_expected_access( E&& e )
        noexcept( std::is_nothrow_move_constructible<E>::value )
        : bad_expected_access<void>( "bad_expected_access<" + boost::core::demangle( typeid(E).name() ) + ">" + variant2::detail::add_value( e ) ), e_( std::move(e) )
    {
    }

    E error() const
    {
        return e_;
    }
};

// throw_on_unexpected

template<class E> void throw_on_unexpected( E const& e )
{
    throw bad_expected_access<E>( e );
}

void throw_on_unexpected( std::error_code const & e )
{
    throw std::system_error( e );
}

void throw_on_unexpected( std::exception_ptr const & e )
{
    if( e )
    {
        std::rethrow_exception( e );
    }
    else
    {
        throw bad_expected_access<>( "bad_expected_access<>: null exception_ptr" );
    }
}

// expected

template<class T, class... E> class expected;

template<class T> struct is_expected: std::false_type {};
template<class T, class... E> struct is_expected<expected<T, E...>>: std::true_type {};

template<class T, class... E> class expected
{
private:

    variant<T, E...> v_;

private:

    void _bad_access() const
    {
        mp_for_index<mp_size<expected>>( v_.index(), [&]( auto I )
        {
            if( I == 0 )
            {
                throw bad_expected_access<>( "bad_expected_access<>: value present on error request" );
            }
            else
            {
                throw_on_unexpected( get<I>(v_) );
            }
        });
    }

public:

    // value constructors

    constexpr expected() noexcept( std::is_nothrow_default_constructible<T>::value )
    {
    }

    constexpr expected( T const& t ) noexcept( std::is_nothrow_copy_constructible<T>::value ): v_( in_place_index<0>, t )
    {
    }

    constexpr expected( T && t ) noexcept( std::is_nothrow_move_constructible<T>::value ): v_( in_place_index<0>, std::move(t) )
    {
    }

    // template<class U> constexpr expected( U && u ); where U in E...?

    // in-place constructor?

    // unexpected constructor

    template<class... E2,
        class En = mp_if<mp_all<std::is_copy_constructible<E2>..., mp_contains<mp_list<E...>, E2>...>, void>>
    constexpr expected( unexpected_<E2...> const & x ): v_( x )
    {
    }

    template<class... E2,
        class En = mp_if<mp_all<std::is_copy_constructible<E2>..., mp_contains<mp_list<E...>, E2>...>, void>>
    constexpr expected( unexpected_<E2...> && x ): v_( std::move(x) )
    {
    }

    // conversion constructor

    template<class... E2,
        class En = mp_if<mp_all<std::is_copy_constructible<E2>..., mp_contains<mp_list<E...>, E2>...>, void>>
    constexpr expected( expected<T, E2...> const & x ): v_( x.v_ )
    {
    }

    template<class... E2,
        class En = mp_if<mp_all<std::is_move_constructible<E2>..., mp_contains<mp_list<E...>, E2>...>, void>>
    constexpr expected( expected<T, E2...> && x ): v_( std::move(x.v_) )
    {
    }

    // emplace

    template<class... A> void emplace( A&&... a )
    {
        v_.emplace( std::forward<A>(a)... );
    }

    template<class V, class... A> void emplace( std::initializer_list<V> il, A&&... a )
    {
        v_.emplace( il, std::forward<A>(a)... );
    }

    // swap

    void swap( expected & r ) noexcept( noexcept( v_.swap( r.v_ ) ) )
    {
        v_.swap( r.v_ );
    }

    // value queries

    constexpr bool has_value() const noexcept
    {
        return v_.index() == 0;
    }

    constexpr explicit operator bool() const noexcept
    {
        return v_.index() == 0;
    }

    // checked value access

    constexpr T& value() &
    {
        if( !has_value() )
        {
            _bad_access();
        }

        return *get_if<0>(&v_);
    }

    constexpr T const& value() const&
    {
        if( !has_value() )
        {
            _bad_access();
        }

        return *get_if<0>(&v_);
    }

    constexpr T&& value() &&
    {
        return std::move( value() );
    }

    constexpr T const&& value() const&&
    {
        return std::move( value() );
    }

    // unchecked value access

    T* operator->() noexcept
    {
        return get_if<0>(&v_);
    }

    T const* operator->() const noexcept
    {
        return get_if<0>(&v_);
    }

    T& operator*() & noexcept
    {
        T* p = get_if<0>(&v_);

        assert( p != 0 );

        return *p;
    }

    T const& operator*() const & noexcept
    {
        T const* p = get_if<0>(&v_);

        assert( p != 0 );

        return *p;
    }

    T&& operator*() && noexcept
    {
        return std::move(**this);
    }

    T const&& operator*() const && noexcept
    {
        return std::move(**this);
    }

    // error queries

    template<class E2> constexpr bool has_error() const noexcept
    {
        using I = mp_find<expected, E2>;
        return v_.index() == I::value;
    }

    constexpr bool has_error() const noexcept
    {
        static_assert( sizeof...(E) == 1, "has_error() is only valid when there is a single E" );
        return has_error<mp_first<expected>>();
    }

    // error access

    unexpected_<E...> unexpected() const
    {
        if( has_value() )
        {
            _bad_access();
        }

        return v_.template subset<E...>();
    }

    template<class E2> constexpr E2 error() const noexcept
    {
        using I = mp_find<expected, E2>;

        if( v_.index() != I::value )
        {
            _bad_access();
        }

        return get<I>( v_ );
    }

    constexpr mp_first<expected> error() const noexcept
    {
        static_assert( sizeof...(E) == 1, "error() is only valid when there is a single E" );
        return error<mp_first<expected>>();
    }

    // error mapping

private:

    template<class F> struct Qret
    {
        template<class... A> using fn = decltype( std::declval<F>()( std::declval<A>()... ) );
    };

    template<class F> using remapped = mp_append<expected<T>, mp_unique<mp_transform_q<Qret<F>, mp_list<E...>>>>;

    template<class R, std::size_t I, class F, class V> static R _remap_error( mp_size_t<I>, F && f, V && v )
    {
        // return R( std::forward<F>(f)( std::forward<V>(v) ) );

        auto e = std::forward<F>(f)( std::forward<V>(v) );

        return unexpected_<decltype(e)>{ e };
    }

    template<class R, class F, class V> static R _remap_error( mp_size_t<0>, F && /*f*/, V && v )
    {
        return R( std::forward<V>(v) );
    }

public:

    template<class F> remapped<F> remap_errors( F && f ) const
    {
        using R = remapped<F>;

        return mp_for_index<mp_size<expected>>( v_.index(), [&]( auto I ) {

            return _remap_error<R>( I, f, get<I>(v_) );

        });
    }

    expected<T, std::error_code> remap_errors()
    {
        using R = expected<T, std::error_code>;

        auto f = []( auto const& e ){ return make_error_code(e); };

        return mp_for_index<mp_size<expected>>( v_.index(), [&]( auto I ) {

            return _remap_error<R>( I, f, get<I>(v_) );

        });
    }

    // then

private:

    template<class F, class U> using then_result_ = decltype( std::declval<F>()( std::declval<U>() ) );

    template<class F, class U, class R = then_result_<F, U>> using then_result = mp_if<is_expected<R>, R, expected<R, E...>>;

public:

    template<class F> then_result<F, T const&> then( F && f ) const
    {
        if( has_value() )
        {
            return std::forward<F>(f)( **this );
        }
        else
        {
            return unexpected();
        }
    }
};

template<class T, class... E> inline constexpr bool operator==( expected<T, E...> const & x1, expected<T, E...> const & x2 )
{
    return x1.v_ == x2.v_;
}

template<class T, class... E> inline constexpr bool operator!=( expected<T, E...> const & x1, expected<T, E...> const & x2 )
{
    return x1.v_ != x2.v_;
}

template<class T, class... E> inline void swap( expected<T, E...> & x1, expected<T, E...> & x2 ) noexcept( noexcept( x1.swap( x2 ) ) )
{
    x1.swap( x2 );
}

} // namespace variant2
} // namespace boost

#endif // #ifndef BOOST_VARIANT2_EXPECTED_HPP_INCLUDED

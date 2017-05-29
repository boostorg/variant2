#ifndef BOOST_VARIANT2_VARIANT_HPP_INCLUDED
#define BOOST_VARIANT2_VARIANT_HPP_INCLUDED

//  Copyright 2015-2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/mp11.hpp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <cstddef>
#include <type_traits>
#include <exception>
#include <cassert>

//

namespace boost
{
namespace variant2
{

using namespace boost::mp11;

// bad_variant_access

class bad_variant_access: public std::exception
{
public:

    bad_variant_access() noexcept
    {
    }

    char const * what() const noexcept
    {
        return "bad_variant_access";
    }
};

//

template<class... T> class variant;

// variant_size

template<class T> struct variant_size
{
};

template<class T> struct variant_size<T const>: variant_size<T>
{
};

template<class T> struct variant_size<T volatile>: variant_size<T>
{
};

template<class T> struct variant_size<T const volatile>: variant_size<T>
{
};

template <class T> /*inline*/ constexpr std::size_t variant_size_v = variant_size<T>::value;

template <class... T> struct variant_size<variant<T...>>: mp_size<variant<T...>>
{
};

// variant_alternative

template<std::size_t I, class T> struct variant_alternative;

template<std::size_t I, class T> using variant_alternative_t = typename variant_alternative<I, T>::type;

namespace detail
{
    template<class I, class T, class Q> using var_alt_t = mp_invoke<Q, variant_alternative_t<I::value, T>>;
} // namespace detail

template<std::size_t I, class T> struct variant_alternative
{
};

template<std::size_t I, class T> struct variant_alternative<I, T const>: mp_defer< variant2::detail::var_alt_t, mp_size_t<I>, T, mp_quote<std::add_const_t>>
{
};

template<std::size_t I, class T> struct variant_alternative<I, T volatile>: mp_defer< variant2::detail::var_alt_t, mp_size_t<I>, T, mp_quote<std::add_volatile_t>>
{
};

template<std::size_t I, class T> struct variant_alternative<I, T const volatile>: mp_defer< variant2::detail::var_alt_t, mp_size_t<I>, T, mp_quote<std::add_cv_t>>
{
};

template<std::size_t I, class... T> struct variant_alternative<I, variant<T...>>: mp_defer<mp_at, variant<T...>, mp_size_t<I>>
{
};

// holds_alternative

template<class U, class... T> constexpr bool holds_alternative( variant<T...> const& v ) noexcept
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    return v.index() == mp_find<variant<T...>, U>::value;
}

// get

template<std::size_t I, class... T> constexpr variant_alternative_t<I, variant<T...>>& get(variant<T...>& v)
{
    static_assert( I < sizeof...(T), "Index out of bounds" );

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1910 )

    return v.index() == I? v._get_impl( mp_size_t<I>() ): ( throw bad_variant_access(), v._get_impl( mp_size_t<I>() ) );

#else

    return v.index() == I? v._get_impl( mp_size_t<I>() ): throw bad_variant_access();

#endif
}

template<std::size_t I, class... T> constexpr variant_alternative_t<I, variant<T...>>&& get(variant<T...>&& v)
{
    static_assert( I < sizeof...(T), "Index out of bounds" );

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1910 )

    return std::move( v.index() == I? v._get_impl( mp_size_t<I>() ): ( throw bad_variant_access(), v._get_impl( mp_size_t<I>() ) ) );

#else

    return v.index() == I? std::move( v._get_impl( mp_size_t<I>() ) ): throw bad_variant_access();

#endif
}

template<std::size_t I, class... T> constexpr variant_alternative_t<I, variant<T...> const>& get(variant<T...> const& v)
{
    static_assert( I < sizeof...(T), "Index out of bounds" );

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1910 )

    return v.index() == I? v._get_impl( mp_size_t<I>() ): ( throw bad_variant_access(), v._get_impl( mp_size_t<I>() ) );

#else

    return v.index() == I? v._get_impl( mp_size_t<I>() ): throw bad_variant_access();

#endif
}

template<std::size_t I, class... T> constexpr variant_alternative_t<I, variant<T...> const>&& get(variant<T...> const&& v)
{
    static_assert( I < sizeof...(T), "Index out of bounds" );

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1910 )

    return std::move( v.index() == I? v._get_impl( mp_size_t<I>() ): ( throw bad_variant_access(), v._get_impl( mp_size_t<I>() ) ) );

#else

    return v.index() == I? std::move( v._get_impl( mp_size_t<I>() ) ): throw bad_variant_access();

#endif
}

// get

template<class U, class... T> constexpr U& get(variant<T...>& v)
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v.index() == I? v._get_impl( mp_size_t<I>() ): throw bad_variant_access();
}

template<class U, class... T> constexpr U&& get(variant<T...>&& v)
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v.index() == I? std::move( v._get_impl( mp_size_t<I>() ) ): throw bad_variant_access();
}

template<class U, class... T> constexpr U const& get(variant<T...> const& v)
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v.index() == I? v._get_impl( mp_size_t<I>() ): throw bad_variant_access();
}

template<class U, class... T> constexpr U const&& get(variant<T...> const&& v)
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v.index() == I? std::move( v._get_impl( mp_size_t<I>() ) ): throw bad_variant_access();
}

// get_if

template<std::size_t I, class... T> constexpr std::add_pointer_t<variant_alternative_t<I, variant<T...>>> get_if(variant<T...>* v) noexcept
{
    static_assert( I < sizeof...(T), "Index out of bounds" );
    return v->index() == I? &v->_get_impl( mp_size_t<I>() ): 0;
}

template<std::size_t I, class... T> constexpr std::add_pointer_t<const variant_alternative_t<I, variant<T...>>> get_if(variant<T...> const * v) noexcept
{
    static_assert( I < sizeof...(T), "Index out of bounds" );
    return v->index() == I? &v->_get_impl( mp_size_t<I>() ): 0;
}

template<class U, class... T> constexpr std::add_pointer_t<U> get_if(variant<T...>* v) noexcept
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v->index() == I? &v->_get_impl( mp_size_t<I>() ): 0;
}

template<class U, class... T> constexpr std::add_pointer_t<U const> get_if(variant<T...> const * v) noexcept
{
    static_assert( mp_count<variant<T...>, U>::value == 1, "The type must occur exactly once in the list of variant alternatives" );
    constexpr auto I = mp_find<variant<T...>, U>::value;

    return v->index() == I? &v->_get_impl( mp_size_t<I>() ): 0;
}

//

namespace detail
{

// variant_storage

template<class D, class... T> union variant_storage_impl;

template<class... T> using variant_storage = variant_storage_impl<mp_all<std::is_trivially_destructible<T>...>, T...>;

template<class D> union variant_storage_impl<D>
{
};

// not all trivially destructible
template<class T1, class... T> union variant_storage_impl<mp_false, T1, T...>
{
    T1 first_;
    variant_storage<T...> rest_;

    template<class... A> constexpr explicit variant_storage_impl( mp_size_t<0>, A&&... a ): first_( std::forward<A>(a)... )
    {
    }

    template<std::size_t I, class... A> constexpr explicit variant_storage_impl( mp_size_t<I>, A&&... a ): rest_( mp_size_t<I-1>(), std::forward<A>(a)... )
    {
    }

    ~variant_storage_impl()
    {
    }

    template<class... A> void emplace( mp_size_t<0>, A&&... a ) noexcept
    {
        ::new( &first_ ) T1( std::forward<A>(a)... );
    }

    template<std::size_t I, class... A> void emplace( mp_size_t<I>, A&&... a ) noexcept
    {
        rest_.emplace( mp_size_t<I-1>(), std::forward<A>(a)... );
    }

    constexpr T1& get( mp_size_t<0> ) noexcept { return first_; }
    constexpr T1 const& get( mp_size_t<0> ) const noexcept { return first_; }

    template<std::size_t I> constexpr mp_at_c<mp_list<T...>, I-1>& get( mp_size_t<I> ) noexcept { return rest_.get( mp_size_t<I-1>() ); }
    template<std::size_t I> constexpr mp_at_c<mp_list<T...>, I-1> const& get( mp_size_t<I> ) const noexcept { return rest_.get( mp_size_t<I-1>() ); }
};

// all trivially destructible
template<class T1, class... T> union variant_storage_impl<mp_true, T1, T...>
{
    T1 first_;
    variant_storage<T...> rest_;

    template<class... A> constexpr explicit variant_storage_impl( mp_size_t<0>, A&&... a ): first_( std::forward<A>(a)... )
    {
    }

    template<std::size_t I, class... A> constexpr explicit variant_storage_impl( mp_size_t<I>, A&&... a ): rest_( mp_size_t<I-1>(), std::forward<A>(a)... )
    {
    }

    template<class... A> void emplace( mp_size_t<0>, A&&... a ) noexcept
    {
        ::new( &first_ ) T1( std::forward<A>(a)... );
    }

    template<std::size_t I, class... A> void emplace( mp_size_t<I>, A&&... a ) noexcept
    {
        rest_.emplace( mp_size_t<I-1>(), std::forward<A>(a)... );
    }

    constexpr T1& get( mp_size_t<0> ) noexcept { return first_; }
    constexpr T1 const& get( mp_size_t<0> ) const noexcept { return first_; }

    template<std::size_t I> constexpr mp_at_c<mp_list<T...>, I-1>& get( mp_size_t<I> ) noexcept { return rest_.get( mp_size_t<I-1>() ); }
    template<std::size_t I> constexpr mp_at_c<mp_list<T...>, I-1> const& get( mp_size_t<I> ) const noexcept { return rest_.get( mp_size_t<I-1>() ); }
};

// resolve_overload_*

template<class... T> struct overload;

template<> struct overload<>
{
    void operator()() const;
};

template<class T1, class... T> struct overload<T1, T...>: overload<T...>
{
    using overload<T...>::operator();
    mp_identity<T1> operator()(T1) const;
};

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1910 )

template<class U, class... T> struct resolve_overload_type_impl
{
    using type = decltype( overload<T...>()(std::declval<U>()) );
};

template<class U, class... T> using resolve_overload_type = typename resolve_overload_type_impl<U, T...>::type::type;

#else

template<class U, class... T> using resolve_overload_type = typename decltype( overload<T...>()(std::declval<U>()) )::type;

#endif

template<class U, class... T> using resolve_overload_index = mp_find<mp_list<T...>, resolve_overload_type<U, T...>>;

// variant_base

template<class D1, class D2, class... T> struct variant_base_impl; // trivially destructible, single buffered
template<class... T> using variant_base = variant_base_impl<mp_all<std::is_trivially_destructible<T>...>, mp_all<std::is_nothrow_move_constructible<T>...>, T...>;

struct none {};

// trivially destructible, single buffered
template<class... T> struct variant_base_impl<mp_true, mp_true, T...>
{
    int ix_;
    variant_storage<none, T...> st1_;

    constexpr variant_base_impl(): ix_( 0 ), st1_( mp_size_t<0>() )
    {
    }

    template<class I, class... A> constexpr explicit variant_base_impl( I, A&&... a ): ix_( I::value + 1 ), st1_( mp_size_t<I::value + 1>(), std::forward<A>(a)... )
    {
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I>& _get_impl( mp_size_t<I> i ) noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J );

        return st1_.get( mp_size_t<J>() );
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I> const& _get_impl( mp_size_t<I> i ) const noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J );

        return st1_.get( mp_size_t<J>() );
    }

    template<std::size_t I, class... A> void emplace( A&&... a )
    {
        size_t const J = I+1;

        using U = mp_at_c<variant<T...>, I>;

        if( std::is_nothrow_constructible<U, A...>::value )
        {
            st1_.emplace( mp_size_t<J>(), std::forward<A>(a)... );
            ix_ = J;
        }
        else
        {
            U tmp( std::forward<A>(a)... );

            st1_.emplace( mp_size_t<J>(), std::move(tmp) );
            ix_ = J;
        }
    }
};

// trivially destructible, double buffered
template<class... T> struct variant_base_impl<mp_true, mp_false, T...>
{
    int ix_;
    variant_storage<none, T...> st1_;
    variant_storage<none, T...> st2_;

    constexpr variant_base_impl(): ix_( 0 ), st1_( mp_size_t<0>() ), st2_( mp_size_t<0>() )
    {
    }

    template<class I, class... A> constexpr explicit variant_base_impl( I, A&&... a ): ix_( I::value + 1 ), st1_( mp_size_t<I::value + 1>(), std::forward<A>(a)... ), st2_( mp_size_t<0>() )
    {
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I>& _get_impl( mp_size_t<I> i ) noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J || -ix_ == J );

        constexpr mp_size_t<J> j{};
        return ix_ >= 0? st1_.get( j ): st2_.get( j );
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I> const& _get_impl( mp_size_t<I> i ) const noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J || -ix_ == J );

        constexpr mp_size_t<J> j{};
        return ix_ >= 0? st1_.get( j ): st2_.get( j );
    }

    template<std::size_t I, class... A> void emplace( A&&... a )
    {
        size_t const J = I+1;

        if( ix_ >= 0 )
        {
            st2_.emplace( mp_size_t<J>(), std::forward<A>(a)... );
            ix_ = -static_cast<int>( J );
        }
        else
        {
            st1_.emplace( mp_size_t<J>(), std::forward<A>(a)... );
            ix_ = J;
        }
    }
};

// not trivially destructible, single buffered
template<class... T> struct variant_base_impl<mp_false, mp_true, T...>
{
    int ix_;
    variant_storage<none, T...> st1_;

    constexpr variant_base_impl(): ix_( 0 ), st1_( mp_size_t<0>() )
    {
    }

    template<class I, class... A> constexpr explicit variant_base_impl( I, A&&... a ): ix_( I::value + 1 ), st1_( mp_size_t<I::value + 1>(), std::forward<A>(a)... )
    {
    }

    void _destroy() noexcept
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            using U = mp_at_c<variant<T...>, I>;
            constexpr auto J = decltype(I)::value + 1;

            if( J == ix_ )
            {
                st1_.get( mp_size_t<J>() ).~U();
            }

        });
    }

    ~variant_base_impl() noexcept
    {
        _destroy();
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I>& _get_impl( mp_size_t<I> i ) noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J );

        return st1_.get( mp_size_t<J>() );
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I> const& _get_impl( mp_size_t<I> i ) const noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J );

        return st1_.get( mp_size_t<J>() );
    }

    template<std::size_t I, class... A> void emplace( A&&... a )
    {
        size_t const J = I+1;

        using U = mp_at_c<variant<T...>, I>;

        if( std::is_nothrow_constructible<U, A...>::value )
        {
            _destroy();
            st1_.emplace( mp_size_t<J>(), std::forward<A>(a)... );

            ix_ = J;
        }
        else
        {
            assert( std::is_nothrow_move_constructible<U>::value );

            U tmp( std::forward<A>(a)... );

            _destroy();
            st1_.emplace( mp_size_t<J>(), std::move(tmp) );

            ix_ = J;
        }
    }
};

// not trivially destructible, double buffered
template<class... T> struct variant_base_impl<mp_false, mp_false, T...>
{
    int ix_;
    variant_storage<none, T...> st1_;
    variant_storage<none, T...> st2_;

    constexpr variant_base_impl(): ix_( 0 ), st1_( mp_size_t<0>() ), st2_( mp_size_t<0>() )
    {
    }

    template<class I, class... A> constexpr explicit variant_base_impl( I, A&&... a ): ix_( I::value + 1 ), st1_( mp_size_t<I::value + 1>(), std::forward<A>(a)... ), st2_( mp_size_t<0>() )
    {
    }

    void _destroy() noexcept
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            using U = mp_at_c<variant<T...>, I>;
            constexpr auto J = decltype(I)::value + 1;

            if( ix_ > 0 && J == ix_ )
            {
                st1_.get( mp_size_t<J>() ).~U();
            }

            if( ix_ < 0 && J == -ix_ )
            {
                st2_.get( mp_size_t<J>() ).~U();
            }

        });
    }

    ~variant_base_impl() noexcept
    {
        _destroy();
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I>& _get_impl( mp_size_t<I> i ) noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J || -ix_ == J );

        constexpr mp_size_t<J> j{};
        return ix_ >= 0? st1_.get( j ): st2_.get( j );
    }

    template<std::size_t I> constexpr mp_at_c<variant<T...>, I> const& _get_impl( mp_size_t<I> i ) const noexcept
    {
        size_t const J = I+1;

        assert( ix_ == J || -ix_ == J );

        constexpr mp_size_t<J> j{};
        return ix_ >= 0? st1_.get( j ): st2_.get( j );
    }

    template<std::size_t I, class... A> void emplace( A&&... a )
    {
        size_t const J = I+1;

        if( ix_ >= 0 )
        {
            st2_.emplace( mp_size_t<J>(), std::forward<A>(a)... );
            _destroy();

            ix_ = -static_cast<int>( J );
        }
        else
        {
            st1_.emplace( mp_size_t<J>(), std::forward<A>(a)... );
            _destroy();

            ix_ = J;
        }
    }
};

} // namespace detail

// in_place_type_t

template<class T> struct in_place_type_t
{
};

// in_place_index_t

template<std::size_t I> struct in_place_index_t
{
};

// variant

template<class... T> class variant: private variant2::detail::variant_base<T...>
{
private:

    using variant_base = variant2::detail::variant_base<T...>;

public:

    // constructors

    constexpr variant()
        noexcept( std::is_nothrow_default_constructible< mp_first<variant<T...>> >::value )
        : variant_base( mp_size_t<0>() )
    {
    }

    variant( variant const& r )
        noexcept( mp_all<std::is_nothrow_copy_constructible<T>...>::value )
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            if( I == r.index() )
            {
                ::new( static_cast<variant_base*>(this) ) variant_base( I, r._get_impl( I ) );
            }

        });
    }

    variant( variant && r )
        noexcept( mp_all<std::is_nothrow_move_constructible<T>...>::value )
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            if( I == r.index() )
            {
                ::new( static_cast<variant_base*>(this) ) variant_base( I, std::move( r._get_impl( I ) ) );
            }

        });
    }

    template<class U> constexpr variant( U&& u )
        noexcept( std::is_nothrow_constructible< variant2::detail::resolve_overload_type<U&&, T...>, U >::value )
        : variant_base( variant2::detail::resolve_overload_index<U&&, T...>(), std::forward<U>(u) )
    {
    }

    template<class U, class... A, class I = mp_find<variant<T...>, U>>
    constexpr explicit variant( in_place_type_t<U>, A&&... a ): variant_base( I(), std::forward<A>(a)... )
    {
    }

    template<class U, class V, class... A>
    constexpr explicit variant( in_place_type_t<U>, std::initializer_list<V>, A&&... a );

    template<std::size_t I, class... A>
    constexpr explicit variant( in_place_index_t<I>, A&&... a ): variant_base( mp_size_t<I>(), std::forward<A>(a)... )
    {
    }

    template<std::size_t I, class V, class... A>
    constexpr explicit variant( in_place_index_t<I>, std::initializer_list<V>, A&&... );

    // assignment
    variant& operator=( variant const & r )
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            constexpr auto J = decltype(I)::value;

            if( J == r.index() )
            {
                if( index() == J )
                {
                    get<J>(*this) = get<J>(r);
                }
                else
                {
                    variant_base::template emplace<J>( get<J>(r) );
                }
            }

        });

        return *this;
    }

    variant& operator=( variant&& r ) noexcept( mp_all<std::is_nothrow_move_constructible<T>..., std::is_nothrow_move_assignable<T>...>::value )
    {
        mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

            constexpr auto J = decltype(I)::type::value;

            if( J == r.index() )
            {
                if( index() == J )
                {
                    get<J>(*this) = get<J>(std::move(r));
                }
                else
                {
                    variant_base::template emplace<J>( get<J>(std::move(r)) );
                }
            }

        });

        return *this;
    }

    template<class U,
        class E1 = std::enable_if_t<!std::is_same<std::decay_t<U>, variant>::value>,
        class V = variant2::detail::resolve_overload_type<U, T...>,
        class E2 = std::enable_if_t<std::is_assignable<V&, U>::value && std::is_constructible<V&, U>::value>
        >
        variant& operator=( U&& u ) // noexcept(see below );
    {
        std::size_t const I = variant2::detail::resolve_overload_index<U, T...>::value;

        if( index() == I )
        {
            _get_impl( mp_size_t<I>() ) = std::forward<U>(u);
        }
        else
        {
            this->template emplace<I>( std::forward<U>(u) );
        }

        return *this;
    }

    // modifiers
    // using variant_base::emplace;

    template<class U, class... A, class I = mp_find<variant<T...>, U>> U& emplace( A&&... a )
    {
        variant_base::template emplace<I::value>( std::forward<A>(a)... );
        return _get_impl( I() );
    }

    template<class U, class V, class... A> U& emplace( std::initializer_list<V> il, A&&... a );

    template<std::size_t I, class... A> variant_alternative_t<I, variant<T...>>& emplace( A&&... a )
    {
        variant_base::template emplace<I>( std::forward<A>(a)... );
        return _get_impl( mp_size_t<I>() );
    }

    template<std::size_t I, class V, class... A> variant_alternative_t<I, variant<T...>>& emplace( std::initializer_list<V> il, A&&... a );

    // value status

    constexpr size_t index() const noexcept
    {
        return this->ix_ >= 0? this->ix_ - 1 : -this->ix_ - 1;
    }

    // swap

    void swap( variant& r ); // noexcept( ... )

    // private accessors

    constexpr int _real_index() const noexcept
    {
        return this->ix_;
    }

    using variant_base::_get_impl;
};

// relational operators
template<class... T> constexpr bool operator==( variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() != w.index() ) return false;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) == get<I>(w);

    });

    assert( false );
    return false;
}

template<class... T> constexpr bool operator!=( variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() != w.index() ) return true;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) != get<I>(w);

    });

    assert( false );
    return true;
}

template<class... T> constexpr bool operator<( variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() < w.index() ) return true;
    if( v.index() > w.index() ) return false;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) < get<I>(w);

    });

    assert( false );
    return false;
}

template<class... T> constexpr bool operator>(  variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() > w.index() ) return true;
    if( v.index() < w.index() ) return false;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) > get<I>(w);

    });

    assert( false );
    return false;
}

template<class... T> constexpr bool operator<=( variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() < w.index() ) return true;
    if( v.index() > w.index() ) return false;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) <= get<I>(w);

    });

    assert( false );
    return false;
}

template<class... T> constexpr bool operator>=( variant<T...> const & v, variant<T...> const & w )
{
    if( v.index() > w.index() ) return true;
    if( v.index() < w.index() ) return false;

    mp_for_each<mp_iota_c<sizeof...(T)>>([&]( auto I ){

        if( I == v.index() ) return get<I>(v) >= get<I>(w);

    });

    assert( false );
    return false;
}

// visitation
template<class Visitor, class... Variants> constexpr void visit( Visitor&&, Variants&&... );

// specialized algorithms
template<class... T> void swap( variant<T...> & v, variant<T...> & w ); // noexcept(see below );

} // namespace variant2
} // namespace boost

#endif // #ifndef BOOST_VARIANT2_VARIANT_HPP_INCLUDED

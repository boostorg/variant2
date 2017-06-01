#ifndef BOOST_VARIANT2_RESULT_HPP_INCLUDED
#define BOOST_VARIANT2_RESULT_HPP_INCLUDED

//  Copyright 2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_VARIANT2_VARIANT_HPP_INCLUDED
#include <boost/variant2/variant.hpp>
#endif
#include <system_error>
#include <type_traits>
#include <cassert>

//

namespace boost
{
namespace variant2
{

enum class result_errc
{
    not_initialized
};

class result_error_category: public std::error_category
{
public:

    virtual const char * name() const noexcept
    {
        return "boost::variant2::result";
    }

    virtual std::string message( int e ) const
    {
        switch( e )
        {
        case (int)result_errc::not_initialized:

            return "result<> not initialized";

        default:

            return "unknown result<> error";
        }
    }

    static result_error_category const & instance()
    {
        static result_error_category cat;
        return cat;
    }
};

std::error_code make_error_code( result_errc e )
{
    return std::error_code( static_cast<int>( e ), result_error_category::instance() );
}

template<class T> class result
{
private:

    variant<T, std::error_code> v_;

public:

    // constructors

    constexpr result() noexcept: v_( make_error_code( result_errc::not_initialized ) )
    {
    }

    constexpr result( T const& t ): v_( t )
    {
    }

    constexpr result( T&& t ): v_( std::move(t) )
    {
    }

    constexpr result( std::error_code const & ec ) noexcept: v_( ec )
    {
    }

    // queries

    constexpr bool has_value() const noexcept
    {
        return v_.index() == 0;
    }

    constexpr bool has_error() const noexcept
    {
        return v_.index() == 1;
    }

    constexpr explicit operator bool() const noexcept
    {
        return v_.index() == 0;
    }

    // checked value access

    constexpr T& value() &
    {
        if( has_value() )
        {
            return *get_if<0>(&v_);
        }
        else
        {
            throw std::system_error( *get_if<1>(&v_) );
        }
    }

    constexpr T const& value() const&
    {
        if( has_value() )
        {
            return *get_if<0>(&v_);
        }
        else
        {
            throw std::system_error( *get_if<1>(&v_) );
        }
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

    // error access

    /*constexpr*/ std::error_code error() const
    {
        if( has_error() )
        {
            return *get_if<1>(&v_);
        }
        else
        {
            return std::error_code();
        }
    }

    // setters

    void set_value( T const& t ) noexcept( std::is_nothrow_copy_constructible<T>::value )
    {
        v_.emplace<0>( t );
    }

    void set_value( T&& t ) noexcept( std::is_nothrow_move_constructible<T>::value )
    {
        v_.emplace<0>( std::move( t ) );
    }

    void set_error( std::error_code const & e ) noexcept
    {
        v_.emplace<1>( e );
    }

    // swap

    void swap( result& r ) noexcept( noexcept( v_.swap( r.v_ ) ) )
    {
        v_.swap( r.v_ );
    }
};

} // namespace variant2
} // namespace boost

#endif // #ifndef BOOST_VARIANT2_RESULT_HPP_INCLUDED

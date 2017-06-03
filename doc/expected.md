# expected<T, E...>

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
    void throw_on_unexpected( std::error_code const & e );
    void throw_on_unexpected( std::exception_ptr const & e );

    // expected

    template<class T, class... E> class expected
    {
    public:

        // value constructors

        constexpr expected() noexcept( /*see below*/ );

        constexpr expected( T const& t ) noexcept( /*see below*/ );
        constexpr expected( T && t ) noexcept( /*see below*/ );

        // unexpected constructor

        template<class... E2>
        constexpr expected( unexpected_<E2...> const & x );

        template<class... E2>
        constexpr expected( unexpected_<E2...> && x );

        // conversion constructor

        template<class... E2>
        constexpr expected( expected<T, E2...> const & x );

        template<class... E2>
        constexpr expected( expected<T, E2...> && x );

        // emplace

        template<class... A> void emplace( A&&... a );
        template<class V, class... A> void emplace( std::initializer_list<V> il, A&&... a );

        // swap

        void swap( expected & r ) noexcept( /*see below*/ );

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
        constexpr mp_first<expected> error() const noexcept;

        // error mapping

        template<class F> /*see below*/ remap_errors( F && f ) const;
        expected<T, std::error_code> remap_errors();

        // then

        template<class F> /*see below*/ operator>>( F && f ) const;
    };

    template<class T, class... E> inline constexpr bool operator==( expected<T, E...> const & x1, expected<T, E...> const & x2 );
    template<class T, class... E> inline constexpr bool operator!=( expected<T, E...> const & x1, expected<T, E...> const & x2 );

    template<class T, class... E> inline void swap( expected<T, E...> & x1, expected<T, E...> & x2 ) noexcept( /*see below*/ );

    } // namespace variant2
    } // namespace boost

    // is_expected

    template<class T> struct is_expected;

## Reference

...

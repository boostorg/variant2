#include <boost/variant2/variant.hpp>
#include <boost/config/pragma_message.hpp>
#include <functional>

#if !( defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L )

BOOST_PRAGMA_MESSAGE( "Test skipped, because __cpp_impl_three_way_comparison is not >= 201907L" )
int main() {}

#elif !__has_include(<compare>)

BOOST_PRAGMA_MESSAGE( "Test skipped because of !__has_include(<compare>)" )
int main() {}

#else

struct non_comparable
{
};

template<class T> struct wrapper
{
    T t{};
    auto operator<=>(const wrapper&) const = default;
};

int main()
{
    using variant = boost::variant2::variant<non_comparable>;
    using reference_wrapper = std::reference_wrapper<variant>;

    wrapper<reference_wrapper*> x, y;
    (void)(x < y);
}

#endif

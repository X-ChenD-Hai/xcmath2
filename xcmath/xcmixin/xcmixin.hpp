#pragma once
#include <type_traits>
namespace xcmixin {
struct EmptyBase {};

namespace details {
template <typename T>
struct return_type {
    using type = T;
};
template <typename T>
using deref_type = typename T::type;

template <template <typename, typename> class... methods>
struct method_recorder {
    template <template <typename, typename> class... ext_methods>
    using push_back = method_recorder<methods..., ext_methods...>;

    template <template <typename, typename> class... ext_methods>
    using push_front = method_recorder<ext_methods..., methods...>;
    template <typename T>
    struct concat_helper;
    template <typename T>
    using concat = deref_type<concat_helper<T>>;
    template <template <typename, typename> class... ext_methods>
    struct concat_helper<method_recorder<ext_methods...>>
        : return_type<method_recorder<methods..., ext_methods...>> {};
};
template <typename... Ts>
struct recorder_concat_helper;
template <typename... Ts>
using recorder_concat = deref_type<recorder_concat_helper<Ts...>>;

template <>
struct recorder_concat_helper<> : return_type<method_recorder<>> {};
template <typename T>
struct recorder_concat_helper<T> : return_type<T> {};
template <typename T, typename... Ts>
struct recorder_concat_helper<T, Ts...>
    : return_type<typename T::template concat<
          deref_type<recorder_concat_helper<Ts...>>>> {};

template <typename Derived, template <typename, typename> class... methods>
struct impl_methods_helper;
template <typename Derived, template <typename, typename> class... methods>
using impl_methods = deref_type<impl_methods_helper<Derived, methods...>>;
template <typename Derived, template <typename, typename> class method>
struct impl_methods_helper<Derived, method> {
    struct type : method<EmptyBase, Derived> {
        using method_recorder = method_recorder<method>;
    };
};
template <typename Derived, template <typename, typename> class method,
          template <typename, typename> class... methods>
struct impl_methods_helper<Derived, method, methods...> {
    struct type : method<deref_type<impl_methods_helper<Derived, methods...>>,
                         Derived> {
        using method_recorder = method_recorder<method, methods...>;
    };
};

template <typename Derived, typename recorders>
struct impl_methods_recorder_helper;
template <typename Derived, typename... recorders>
using impl_methods_recorders = deref_type<
    impl_methods_recorder_helper<Derived, recorder_concat<recorders...>>>;
template <typename Derived, template <typename, typename> class... methods>
struct impl_methods_recorder_helper<Derived, method_recorder<methods...>> {
    struct type : deref_type<impl_methods_helper<Derived, methods...>> {};
};
template <template <typename, typename> class method, typename recorder>
static constexpr bool has_method = false;

template <template <typename, typename> class method,
          template <typename, typename> class... methods>
static constexpr bool has_method<method, method_recorder<methods...>> =
    (std::is_same_v<method<void, void>, methods<void, void>> || ...);

template <typename Derived, template <typename, typename> class method>
static constexpr bool is_impl_method =
    has_method<method, typename Derived::method_recorder>;

}  // namespace details

using details::has_method;
using details::impl_methods;
using details::impl_methods_recorders;
using details::is_impl_method;
using details::method_recorder;
using details::recorder_concat;

}  // namespace xcmixin

#define xcmixin_require_method(method)                        \
    static_assert(::xcmixin::is_impl_method<Derived, method>, \
                  "Derived must be derived from " #method)

#define XCMIXIN_METHOD_INIT             \
    using base_type = Base;             \
    using Self = std::decay_t<Derived>; \
    using ConstSelf = const Self;
#define XCMIXIN_METHOD_DEF_BEGIN(name)         \
    template <typename Base, typename Derived> \
    struct name : Base {                       \
        METHOD_INIT
#define XCMIXIN_METHOD_DEF_END() \
    }                            \
    ;
#define XCMIXIN_METHOD_DECLARE(name) \
    XCMIXIN_METHOD_DEF_BEGIN(name)   \
    }
#ifdef __GNUC__
#define XCMIXIN_IMPL_METHOD_BEGIN(name, ext_template_params...) \
    template <typename Base, ext_template_params>               \
        struct name < Base,
#define XCMIXIN_IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, \
                                                ext_template_params...)  \
    template <typename Base, ext_template_params>                        \
        requires(require_statement)                                      \
    struct name < Base,
#define XCMIXIN_IMPL_METHOD_FOR(cls...) \
    cls > : Base {                      \
        using Self = cls;               \
        using ConstSelf = const std::remove_const_t<Self>;

#else
#define XCMIXIN_IMPL_METHOD_BEGIN(name, ...) \
    template <typename Base, __VA_ARGS__>    \
        struct name < Base,
#define XCMIXIN_IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, ...) \
    template <typename Base, __VA_ARGS__>                                     \
        requires(require_statement)                                           \
    struct name < Base,
#define XCMIXIN_IMPL_METHOD_FOR(...) \
    __VA_ARGS__ > : Base {           \
        using Self = __VA_ARGS__;    \
        using ConstSelf = const std::remove_const_t<Self>;
#endif
#define XCMIXIN_IMPL_METHOD_END() \
    }                             \
    ;

#define XCMIXIN_FACTORY_DECLARE(name) XCMIXIN_METHOD_DECLARE(name)
#define XCMIXIN_FACTORY_DEF_BEGIN(name) XCMIXIN_METHOD_DEF_BEGIN(name)
#define XCMIXIN_FACTORY_DEF_END() XCMIXIN_METHOD_DEF_END()
#define XCMIXIN_IMPL_FACTORY_BEGIN(name, ...) \
    XCMIXIN_IMPL_METHOD_BEGIN(name, __VA_ARGS__)
#define XCMIXIN_IMPL_FACTORY_BEGIN_WITH_REQUIRES(name, require_statement, ...) \
    XCMIXIN_IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement,           \
                                            __VA_ARGS__)
#define XCMIXIN_IMPL_FACTORY_FOR(...) XCMIXIN_IMPL_METHOD_FOR(__VA_ARGS__)
#define XCMIXIN_IMPL_FACTORY_END() XCMIXIN_IMPL_METHOD_END()
#define xcmixin_self (*static_cast<Self*>(this))
#define xcmixin_const_self (*static_cast<ConstSelf*>(this))
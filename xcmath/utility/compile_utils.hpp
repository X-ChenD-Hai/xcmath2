#pragma once

#ifdef _MSVC_LANG
#define XCMATH_CXX_STD _MSVC_LANG
#else
#define XCMATH_CXX_STD __cplusplus
#endif
#define METHOD_INIT                     \
    using base_type = Base;             \
    using Self = std::decay_t<Derived>; \
    using ConstSelf = const Self;
#define METHOD_DEF_BEGIN(name)                 \
    template <typename Base, typename Derived> \
    struct name : Base {                       \
        METHOD_INIT
#define METHOD_DEF_END() \
    }                    \
    ;
#define METHOD_DECLARE(name) \
    METHOD_DEF_BEGIN(name)   \
    }
#ifdef __GNUC__
#define IMPL_METHOD_BEGIN(name, ext_template_params...) \
    template <typename Base, ext_template_params>       \
        struct name < Base,
#define IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, \
                                        ext_template_params...)  \
    template <typename Base, ext_template_params>                \
        requires(require_statement)                              \
    struct name < Base,
#define IMPL_METHOD_FOR(cls...) \
    cls > : Base {              \
        using Self = cls;       \
        using ConstSelf = const std::remove_const_t<Self>;

#else
#define IMPL_METHOD_BEGIN(name, ...)      \
    template <typename Base, __VA_ARGS__> \
        struct name < Base,
#define IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, ...) \
    template <typename Base, __VA_ARGS__>                             \
        requires(require_statement)                                   \
    struct name < Base,
#define IMPL_METHOD_FOR(...)      \
    __VA_ARGS__ > : Base {        \
        using Self = __VA_ARGS__; \
        using ConstSelf = const std::remove_const_t<Self>;
#endif
#define IMPL_METHOD_END() \
    }                     \
    ;

#define FACTORY_DECLARE(name) METHOD_DECLARE(name)
#define FACTORY_DEF_BEGIN(name) METHOD_DEF_BEGIN(name)
#define FACTORY_DEF_END() METHOD_DEF_END()
#define IMPL_FACTORY_BEGIN(name, ...) IMPL_METHOD_BEGIN(name, __VA_ARGS__)
#define IMPL_FACTORY_BEGIN_WITH_REQUIRES(name, require_statement, ...) \
    IMPL_METHOD_BEGIN_WITH_REQUIRES(name, require_statement, __VA_ARGS__)
#define IMPL_FACTORY_FOR(...) IMPL_METHOD_FOR(__VA_ARGS__)
#define IMPL_FACTORY_END() IMPL_METHOD_END()
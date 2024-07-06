#pragma once

/// The exposer can forcibly access private members of a class without changing
/// any code. It uses macros to construct a function that returns a reference
/// to the target member variable.

namespace exposer {

template <typename T, T Val, typename Tag>
struct Exposer {
    constexpr friend T fetch(Tag) { return Val; }
};

} // namespace exposer

#define COMBINE_IMPL(x, y) x##y

#define HELPER(x) COMBINE_IMPL(Helper_, x)

#define PROTO(x) COMBINE_IMPL(Proto_, x)

#define FORCE_ACCESS_VAR_IMPL(Class, Type, Member, Tag)                   \
    struct HELPER(Tag) {};                                                \
    template struct Exposer<Type(Class::*), &Class::Member, HELPER(Tag)>; \
    constexpr Type Class::* fetch(HELPER(Tag));                           \
    constexpr Type Class##_##Member(Class &&c) {                          \
        return c.*fetch(HELPER(Tag){});                                   \
    }                                                                     \
    constexpr Type& Class##_##Member(Class &c) {                          \
        return c.*fetch(HELPER(Tag){});                                   \
    }                                                                     \
    constexpr const Type& Class##_##Member(const Class &c) {              \
        return c.*fetch(HELPER(Tag){});                                   \
    }

#define FORCE_ACCESS_CONST_VAR_IMPL(Class, Type, Member, Tag)                   \
    struct HELPER(Tag) {};                                                      \
    template struct Exposer<const Type(Class::*), &Class::Member, HELPER(Tag)>; \
    constexpr const Type Class::* fetch(HELPER(Tag));                           \
        constexpr Type Class##_##Member(Class &&c) {                            \
        return c.*fetch(HELPER(Tag){});                                         \
    }                                                                           \
    constexpr const Type& Class##_##Member(Class &c) {                          \
        return c.*fetch(HELPER(Tag){});                                         \
    }                                                                           \
    constexpr const Type& Class##_##Member(const Class &c) {                    \
        return c.*fetch(HELPER(Tag){});                                         \
    }

#define FORCE_ACCESS_FUNC_IMPL(Class, Proto, Member, Tag)                               \
    struct HELPER(Tag) {};                                                              \
    using PROTO(Tag) = Proto;                                                           \
    template struct Exposer<PROTO(Tag)(Class::*), &Class::Member, HELPER(Tag)>;         \
    constexpr PROTO(Tag) Class::* fetch(HELPER(Tag));                                   \
    template <typename C, typename ...Args>                                             \
    requires std::is_same_v<std::remove_reference_t<C>, Class>                          \
    constexpr decltype(auto) Class##_##Member(C &&c, Args &&...args) {                  \
        return (std::forward<C>(c).*fetch(HELPER(Tag){}))(std::forward<Args>(args)...); \
    }

#define FORCE_ACCESS_CONST_FUNC_IMPL(Class, Proto, Member, Tag)                         \
    struct HELPER(Tag) {};                                                              \
    using PROTO(Tag) = Proto const;                                                     \
    template struct Exposer<PROTO(Tag)(Class::*), &Class::Member, HELPER(Tag)>;         \
    constexpr PROTO(Tag) Demo::* fetch(HELPER(Tag));                                    \
    template <typename C, typename ...Args>                                             \
    requires std::is_same_v<std::remove_const_t<std::remove_reference_t<C>>, Class>     \
    constexpr decltype(auto) Class##_##Member(C &&c, Args &&...args) {                  \
        return (std::forward<C>(c).*fetch(HELPER(Tag){}))(std::forward<Args>(args)...); \
    }

#define FORCE_ACCESS_VAR(Class, Type, Member)                   \
    namespace exposer {                                         \
        FORCE_ACCESS_VAR_IMPL(Class, Type, Member, __COUNTER__) \
    }

#define FORCE_ACCESS_CONST_VAR(Class, Type, Member)                   \
    namespace exposer {                                               \
        FORCE_ACCESS_CONST_VAR_IMPL(Class, Type, Member, __COUNTER__) \
    }

#define FORCE_ACCESS_FUNC(Class, Proto, Member)                   \
    namespace exposer {                                           \
        FORCE_ACCESS_FUNC_IMPL(Class, Proto, Member, __COUNTER__) \
    }

#define FORCE_ACCESS_CONST_FUNC(Class, Proto, Member)                   \
    namespace exposer {                                                 \
        FORCE_ACCESS_CONST_FUNC_IMPL(Class, Proto, Member, __COUNTER__) \
    }

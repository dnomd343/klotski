#pragma once

/// The exposer can forcibly access private members of a class without changing
/// any code. It uses macros to construct a function that returns a reference
/// to the target member variable.

#include <type_traits>

// ----------------------------------------------------------------------------------------- //

namespace exposer {

template <typename T, T Val, typename Dummy>
struct Exposer {
    constexpr friend T fetch(Dummy) { return Val; }
};

} // namespace exposer

// ----------------------------------------------------------------------------------------- //

#define CONCAT(x, y) x##y

#define UNIQUE_TAG_IMPL(x) CONCAT(Unique_, x)

#define UNIQUE_TAG UNIQUE_TAG_IMPL(__COUNTER__)

// ----------------------------------------------------------------------------------------- //

#define ACCESS_SVAR_IMPL(Class, Type, Member, Dummy)         \
    struct Dummy {};                                         \
    template struct Exposer<Type(*), &Class::Member, Dummy>; \
    constexpr Type* fetch(Dummy);                            \
    constexpr Type& Class##_##Member() {                     \
        return *fetch(Dummy{});                              \
    }

#define ACCESS_SFUNC_IMPL(Class, Proto, Member, Dummy, FuncTag) \
    struct Dummy {};                                            \
    using FuncTag = Proto;                                      \
    template struct Exposer<FuncTag(*), &Class::Member, Dummy>; \
    constexpr FuncTag* fetch(Dummy);                            \
    template<typename ...Args>                                  \
    constexpr decltype(auto) sfunc(Args &&...args) {            \
        return fetch(Dummy{})(std::forward<Args>(args)...);     \
    }

#define ACCESS_VAR_IMPL(Class, Type, ConstType, Member, Dummy)      \
    struct Dummy {};                                                \
    template struct Exposer<Type(Class::*), &Class::Member, Dummy>; \
    constexpr Type Class::* fetch(Dummy);                           \
    constexpr Type Class##_##Member(Class &&c) {                    \
        return c.*fetch(Dummy{});                                   \
    }                                                               \
    constexpr Type& Class##_##Member(Class &c) {                    \
        return c.*fetch(Dummy{});                                   \
    }                                                               \
    constexpr ConstType& Class##_##Member(const Class &c) {         \
        return c.*fetch(Dummy{});                                   \
    }

#define ACCESS_FUNC_IMPL(Class, Proto, Member, Dummy, FuncTag)                    \
    struct Dummy {};                                                              \
    using FuncTag = Proto;                                                        \
    template struct Exposer<FuncTag(Class::*), &Class::Member, Dummy>;            \
    constexpr FuncTag Class::* fetch(Dummy);                                      \
    template <typename C, typename ...Args>                                       \
    requires std::is_same_v<std::decay_t<C>, Class>                               \
    constexpr decltype(auto) Class##_##Member(C &&c, Args &&...args) {            \
        return (std::forward<C>(c).*fetch(Dummy{}))(std::forward<Args>(args)...); \
    }

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_STATIC_VAR(Class, Type, Member)            \
    namespace exposer {                                   \
        ACCESS_SVAR_IMPL(Class, Type, Member, UNIQUE_TAG) \
    }

#define EXPOSE_STATIC_FUNC(Class, Proto, Member)                        \
    namespace exposer {                                                 \
        ACCESS_SFUNC_IMPL(Class, Proto, Member, UNIQUE_TAG, UNIQUE_TAG) \
    }

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_VAR(Class, Type, Member)                              \
    namespace exposer {                                              \
        ACCESS_VAR_IMPL(Class, Type, const Type, Member, UNIQUE_TAG) \
    }

#define EXPOSE_FUNC(Class, Proto, Member)                              \
    namespace exposer {                                                \
        ACCESS_FUNC_IMPL(Class, Proto, Member, UNIQUE_TAG, UNIQUE_TAG) \
    }

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_CONST_VAR(Class, Type, Member)                              \
    namespace exposer {                                                    \
        ACCESS_VAR_IMPL(Class, const Type, const Type, Member, UNIQUE_TAG) \
    }

#define EXPOSE_CONST_FUNC(Class, Proto, Member)                              \
    namespace exposer {                                                      \
        ACCESS_FUNC_IMPL(Class, Proto const, Member, UNIQUE_TAG, UNIQUE_TAG) \
    }

// ----------------------------------------------------------------------------------------- //

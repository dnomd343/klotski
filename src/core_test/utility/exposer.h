#pragma once

// TODO: move to independent repo

/// The Exposer can forcibly access private members of a class without changing
/// any code, and in most scenarios, it has zero over-cost. This exposer using
/// macros to construct function that return a reference of target variable or
/// calling target member function.

/// It supports member variables, member functions and their const versions, as
/// well as static variables and functions. They are provided via the `EXPOSE_`
/// macro, A few examples are as follows.

///   + EXPOSE_VAR(Demo, int, val)
///   + EXPOSE_CONST_VAR(Demo, int, cval)
///
///   + EXPOSE_FUNC(Demo, int(const std::string &), func)
///   + EXPOSE_CONST_FUNC(Demo, int(const std::string &), cfunc)
///
///   + EXPOSE_STATIC_VAR(Demo, int, sval)
///   + EXPOSE_STATIC_VAR(Demo, const int, scval)
///
///   + EXPOSE_STATIC_FUNC(Demo, int(std::string_view), sfunc)

/// The advantage of Exposer is that it can directly access the private members
/// without changing any content or compilation parameters. However, this is a
/// hack solution and should not be used in formal code, but only in test code.
/// Another simpler but less compatible way is to add the `-fno-access-control`
/// compilation parameter.

#include <type_traits>

// ----------------------------------------------------------------------------------------- //

namespace {
namespace exposer {

template <typename T, T Val, typename Dummy>
struct Exposer {
    constexpr friend T fetch(Dummy) { return Val; }
};

} // namespace exposer
} // namespace

// ----------------------------------------------------------------------------------------- //

#define CONCAT(x, y) x##y

#define UNIQUE_TAG_IMPL(x) CONCAT(Unique_, x)

#define UNIQUE_TAG UNIQUE_TAG_IMPL(__COUNTER__)

// TODO: add GCC warning ignored

#define NS_EXPOSER_START             \
    namespace {                      \
    namespace exposer {              \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-function\"")

#define NS_EXPOSER_END              \
    _Pragma("clang diagnostic pop") \
    }}

// ----------------------------------------------------------------------------------------- //

#define ACCESS_SVAR_IMPL(Class, Type, Member, Dummy)         \
    NS_EXPOSER_START                                         \
    struct Dummy {};                                         \
    template struct Exposer<Type(*), &Class::Member, Dummy>; \
    constexpr Type* fetch(Dummy);                            \
    constexpr Type& Class##_##Member() {                     \
        return *fetch(Dummy{});                              \
    }                                                        \
    NS_EXPOSER_END

#define ACCESS_SFUNC_IMPL(Class, Proto, Member, Dummy, FuncTag) \
    NS_EXPOSER_START                                            \
    struct Dummy {};                                            \
    using FuncTag = Proto;                                      \
    template struct Exposer<FuncTag(*), &Class::Member, Dummy>; \
    constexpr FuncTag* fetch(Dummy);                            \
    template <typename ...Args>                                 \
    constexpr decltype(auto) sfunc(Args &&...args) {            \
        return fetch(Dummy{})(std::forward<Args>(args)...);     \
    }                                                           \
    NS_EXPOSER_END

#define ACCESS_VAR_IMPL(Class, Type, ConstType, Member, Dummy)      \
    NS_EXPOSER_START                                                \
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
    }                                                               \
    NS_EXPOSER_END

#define ACCESS_FUNC_IMPL(Class, Proto, Member, Dummy, FuncTag)                    \
    NS_EXPOSER_START                                                              \
    struct Dummy {};                                                              \
    using FuncTag = Proto;                                                        \
    template struct Exposer<FuncTag(Class::*), &Class::Member, Dummy>;            \
    constexpr FuncTag Class::* fetch(Dummy);                                      \
    template <typename C, typename ...Args>                                       \
    requires std::is_same_v<std::decay_t<C>, Class>                               \
    constexpr decltype(auto) Class##_##Member(C &&c, Args &&...args) {            \
        return (std::forward<C>(c).*fetch(Dummy{}))(std::forward<Args>(args)...); \
    }                                                                             \
    NS_EXPOSER_END

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_STATIC_VAR(Class, Type, Member) \
    ACCESS_SVAR_IMPL(Class, Type, Member, UNIQUE_TAG)

#define EXPOSE_STATIC_FUNC(Class, Proto, Member) \
    ACCESS_SFUNC_IMPL(Class, Proto, Member, UNIQUE_TAG, UNIQUE_TAG)

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_VAR(Class, Type, Member) \
    ACCESS_VAR_IMPL(Class, Type, const Type, Member, UNIQUE_TAG)

#define EXPOSE_FUNC(Class, Proto, Member) \
    ACCESS_FUNC_IMPL(Class, Proto, Member, UNIQUE_TAG, UNIQUE_TAG)

// ----------------------------------------------------------------------------------------- //

#define EXPOSE_CONST_VAR(Class, Type, Member) \
    ACCESS_VAR_IMPL(Class, const Type, const Type, Member, UNIQUE_TAG)

#define EXPOSE_CONST_FUNC(Class, Proto, Member) \
    ACCESS_FUNC_IMPL(Class, Proto const, Member, UNIQUE_TAG, UNIQUE_TAG)

// ----------------------------------------------------------------------------------------- //

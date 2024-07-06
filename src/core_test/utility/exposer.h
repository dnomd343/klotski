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

#define COMBINE(x, y) COMBINE_IMPL(x, y)

#define UNIQUE_TAG COMBINE(Tag, __COUNTER__)

#define FORCE_ACCESS_VAR_IMPL(Class, Type, Member, Tag) \
    namespace exposer { \
        struct Tag {}; \
        template struct Exposer<Type(Class::*), &Class::Member, Tag>; \
        constexpr Type Class::* fetch(Tag); \
        constexpr Type& Class##_##Member(Class &c) { \
            return c.*fetch(Tag{}); \
        } \
        constexpr const Type& Class##_##Member(const Class &c) { \
            return c.*fetch(Tag{}); \
        } \
        constexpr Type Class##_##Member(Class &&c) { \
            return c.*fetch(Tag{}); \
        } \
    }

#define FORCE_ACCESS_VAR(Class, Type, Member) \
    FORCE_ACCESS_VAR_IMPL(Class, Type, Member, UNIQUE_TAG)

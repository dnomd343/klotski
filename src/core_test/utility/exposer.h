#pragma once

/// The exposer can forcibly access private members of a class without changing
/// any code. It uses macros to construct a function that returns a reference
/// to the target member variable.

namespace exposer {

// REF: http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html

//template <typename T>
//struct Exposer {
//    static T ptr;
//};
//
//template <typename T>
//T Exposer<T>::ptr;
//
//template <typename T, T Ptr>
//struct ExposerImpl {
//    static struct Factory {
//        Factory() { Exposer<T>::ptr = Ptr; }
//    } factory;
//};
//
//template <typename T, T Ptr>
//typename ExposerImpl<T, Ptr>::Factory ExposerImpl<T, Ptr>::factory;

//template <typename T>
//constexpr T fetch();

//template <typename T, T Val, int Flag>
template <typename T, T Val, typename Unique>
struct Exposer {
//    constexpr friend T fetch<>() { return Val; }
    constexpr friend T fetch(Unique) { return Val; }
};

} // namespace exposer

#define FORCIBLY_ACCESS(Class, Member, Type)                                   \
    namespace exposer {                                                        \
        template struct ExposerImpl<decltype(&Class::Member), &Class::Member>; \
        inline auto& Class##_##Member(Class &T) {                              \
            return T.*exposer::Exposer<Type Class::*>::ptr;                    \
        }                                                                      \
    }

#define FORCE_ACCESS_VAR(Class, Member, Type, Unique) \
    namespace exposer { \
        struct Unique {}; \
        template struct Exposer<Type (Class::*), &Class::Member, Unique>; \
        constexpr Type Class::* fetch(Unique); \
        Type& Class##_##Member(Class &T) { \
            return T.*fetch(Unique{}); \
        } \
    }

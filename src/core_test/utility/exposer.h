#pragma once

/// The exposer can forcibly access private members of a class without changing
/// any code. It uses macros to construct a function that returns a reference
/// to the target member variable.

namespace exposer {

template <typename T>
struct Exposer {
    static T ptr;
};

template <typename T>
T Exposer<T>::ptr;

template <typename T, T Ptr>
struct ExposerImpl {
    static struct Factory {
        Factory() { Exposer<T>::ptr = Ptr; }
    } factory;
};

template <typename T, T Ptr>
typename ExposerImpl<T, Ptr>::Factory ExposerImpl<T, Ptr>::factory;

} // namespace exposer

#define PRIVATE_ACCESS(Class, Member, Type)                                           \
    template struct ::exposer::ExposerImpl<decltype(&Class::Member), &Class::Member>; \
    Type& access_##Class##_##Member(Class &T) {                                       \
        return T.*exposer::Exposer<Type Class::*>::ptr;                               \
    }

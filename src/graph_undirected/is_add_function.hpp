#pragma once

#include <vector>
#include <functional>

namespace composite::_impl::graph {

    // Naïve class and function,
    struct S {
    };
    struct S2 {
    };

    void onNodeAdded(const S &existing, S &newNode);

    void onEdgeAdded(const S &existing, S2 &newNode);

    // Template trait to check for overload
    template<class...> using void_t = void;

    template<class, class = void>
    struct is_onNodeAdded : std::false_type {
    };

    template<class T1>
    struct is_onNodeAdded<T1, void_t<decltype(onNodeAdded(std::declval<const T1 &>(), std::declval<T1 &>()))>>
            : std::true_type {
    };

    template<class, class, class = void>
    struct is_onEdgeAdded : std::false_type {
    };

    template<class T1, class T2>
    struct is_onEdgeAdded<T1, T2, void_t<decltype(onEdgeAdded(std::declval<const T1 &>(), std::declval<T2 &>()))>>
            : std::true_type {
    };

}

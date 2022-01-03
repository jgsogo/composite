#pragma once

#include <vector>
#include <functional>


namespace composite::_impl {

    // Naïve class and function,
    struct S {
    };

    void onNodeAdded(const std::vector<std::reference_wrapper<S>> &, S &);

    // Template trait to check for overload
    template<class...> using void_t = void;

    template<class, class = void>
    struct is_onNodeAdded : std::false_type {
    };

    template<class T1>
    struct is_onNodeAdded<T1, void_t<decltype(onNodeAdded(std::declval<const std::vector<std::reference_wrapper<T1>> &>(), std::declval<T1 &>()))>>
            : std::true_type {
    };

}

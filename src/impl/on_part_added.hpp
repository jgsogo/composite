#pragma once

#include <vector>
#include <functional>


namespace composite::_impl {

    // Naïve class and function,
    struct S {
    };

    void onPartAdded(const std::vector<std::reference_wrapper<S>> &, S &);

    // Template trait to check for overload
    template<class...> using void_t = void;

    template<class, class, class = void>
    struct is_add_function : std::false_type {
    };

    template<class T1, class T2>
    struct is_add_function<T1, T2, void_t<decltype(onPartAdded(std::declval<const std::vector<std::reference_wrapper<T1>> &>(), std::declval<T2 &>()))>>
            : std::true_type {
    };

}

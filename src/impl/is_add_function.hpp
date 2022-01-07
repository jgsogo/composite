#pragma once

#include <vector>

namespace composite::_impl {

    namespace {
        // Template trait to check for overload
        template<class...> using void_t = void;

        // Naïve class and function,
        struct S {
        };
        struct S2 {
        };

        // Trees
        void onNodeAdded(const std::vector<std::reference_wrapper<S>> &, S &);

        // Graphs
        void onNodeAdded(const S &existing, S &newNode);

        void onEdgeAdded(const S &existing, S2 &newNode);
    }

    namespace tree {
        template<class, class = void>
        struct is_onNodeAdded : std::false_type {
        };

        template<class T1>
        struct is_onNodeAdded<T1, void_t<decltype(onNodeAdded(std::declval<const std::vector<std::reference_wrapper<T1>> &>(), std::declval<T1 &>()))>>
                : std::true_type {
        };
    }

    namespace graph {
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
}

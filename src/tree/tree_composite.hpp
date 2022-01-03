#pragma once

#include "tree.hpp"
#include "../impl/refl.hpp"

/*
 * Trait compose: combine the functionality of two traits to generate a new one
 * */

namespace composite {

    template<typename Tree1, typename... MoreTrees>
    struct TreeCompose {
        using Tree1Type = Tree1;
        using Tree2Type = typename TreeCompose<MoreTrees...>::Tree;
        using MoreTreesType = std::tuple<MoreTrees...>;


        template<typename...Ts1>
        class NodeTypenameT;

        template<typename ... Ts1, typename ... Ts2>
        class NodeTypenameT<std::tuple<Ts1...>, std::tuple<Ts2...>> : public Tree1Type::NodeTypename, public Tree2Type::NodeTypename {
        public:
            explicit NodeTypenameT(Ts1... args1, Ts2... args2) : Tree1Type::NodeTypename(args1...), Tree2Type::NodeTypename(args2...) {};
        };

        // TALK: Ojo! conseguir los parámetros de un constructor (tiene limitaciones, como que encuentra sólo el primer ctor)
        using Tree1NodeTypenameCtorParams = refl::as_tuple<typename Tree1Type::NodeTypename>;
        using Tree2NodeTypenameCtorParams = refl::as_tuple<typename Tree2Type::NodeTypename>;
        static constexpr bool useComposite = Tree1Type::isOnNodeAdded or TreeCompose<MoreTrees...>::useComposite;

        using NodeTypename = NodeTypenameT<Tree1NodeTypenameCtorParams, Tree2NodeTypenameCtorParams>;
        using Tree = Tree<NodeTypename,
                typename std::conditional<useComposite, TreeCompose<Tree1Type, MoreTrees...>, void>::type>;
    };


    template<typename Tree1>
    struct TreeCompose<Tree1> {
        using Tree = Tree1;
        static constexpr bool useComposite = Tree1::isOnNodeAdded;
    };

    template<typename Tree1, typename... MoreTrees>
    inline auto onNodeAdded(const std::vector<std::reference_wrapper<typename TreeCompose<Tree1, MoreTrees...>::NodeTypename>> &g,
                            typename TreeCompose<Tree1, MoreTrees...>::NodeTypename &p) {
        if constexpr(Tree1::isOnNodeAdded) {
            std::vector<std::reference_wrapper<typename Tree1::NodeTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return std::ref((typename Tree1::NodeTypename &) item); });
            onNodeAdded(groups, (typename Tree1::NodeTypename &) p);
        }

        static_assert(!Tree1::isCompose, "This is always false??? Remove next paragraph");
        /*
        if constexpr(Trait1::isCompose) {
            std::vector<std::reference_wrapper<typename Trait1::GroupTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
            onPartAdded<typename Trait1::CompositeTypename::Trait1Type, typename Trait1::CompositeTypename::Trait2Type>(groups,
                                                                                                                (typename Trait1::CompositeTypename::PartTypename &) p);
        }
         */

        if constexpr(sizeof...(MoreTrees) > 1) {
            std::vector<std::reference_wrapper<typename TreeCompose<MoreTrees...>::Tree::NodeTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups),
                           [](auto &item) { return std::ref((typename TreeCompose<MoreTrees...>::Tree::NodeTypename &) item); });
            onNodeAdded<MoreTrees...>(groups, (typename TreeCompose<MoreTrees...>::Tree::NodeTypename &) p);
        }

        if constexpr(sizeof...(MoreTrees) == 1) {
            using TreeType = typename TreeCompose<MoreTrees...>::Tree;
            if constexpr(TreeType::isOnNodeAdded) {
                std::vector<std::reference_wrapper<typename TreeType::NodeTypename>> groups;
                std::transform(g.begin(), g.end(), std::back_inserter(groups),
                               [](auto &item) { return std::ref((typename TreeType::NodeTypename &) item); });
                onNodeAdded(groups, (typename TreeType::NodeTypename &) p);
            }
        }
    }

}

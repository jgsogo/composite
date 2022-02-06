#pragma once

#include "graph_undirected.hpp"
#include "../impl/refl.hpp"

namespace composite {

    template<typename Graph1, typename... MoreGraphs>
    struct GraphUndirectedCompose {
        using Graph1Type = Graph1;
        using Graph2Type = typename GraphUndirectedCompose<MoreGraphs...>::Graph;
        using MoreGraphsType = std::tuple<MoreGraphs...>;


        template<typename...Ts1>
        class NodeTypenameT;

        template<typename ... Ts1, typename ... Ts2>
        class NodeTypenameT<std::tuple<Ts1...>, std::tuple<Ts2...>> : public Graph1Type::NodeTypename, public Graph2Type::NodeTypename {
        public:
            explicit NodeTypenameT(Ts1... args1, Ts2... args2) : Graph1Type::NodeTypename(args1...), Graph2Type::NodeTypename(args2...) {};
        };

        template<typename...Ts1>
        class EdgeTypenameT;

        template<typename ... Ts1, typename ... Ts2>
        class EdgeTypenameT<std::tuple<Ts1...>, std::tuple<Ts2...>> : public Graph1Type::EdgeTypename, public Graph2Type::EdgeTypename {
        public:
            explicit EdgeTypenameT(Ts1... args1, Ts2... args2) : Graph1Type::EdgeTypename(args1...), Graph2Type::EdgeTypename(args2...) {};
        };


        // TALK: Ojo! conseguir los parámetros de un constructor (tiene limitaciones, como que encuentra sólo el primer ctor)
        using Graph1NodeTypenameCtorParams = refl::as_tuple<typename Graph1Type::NodeTypename>;
        using Graph2NodeTypenameCtorParams = refl::as_tuple<typename Graph2Type::NodeTypename>;
        using Graph1EdgeTypenameCtorParams = refl::as_tuple<typename Graph1Type::EdgeTypename>;
        using Graph2EdgeTypenameCtorParams = refl::as_tuple<typename Graph2Type::EdgeTypename>;
        static constexpr bool useComposite = Graph1Type::isOnNodeAdded or Graph1Type::isOnEdgeAdded or GraphUndirectedCompose<MoreGraphs...>::useComposite;

        using NodeTypename = NodeTypenameT<Graph1NodeTypenameCtorParams, Graph2NodeTypenameCtorParams>;
        using EdgeTypename = EdgeTypenameT<Graph1EdgeTypenameCtorParams, Graph2EdgeTypenameCtorParams>;
        using Graph = ::composite::GraphUndirected<NodeTypename, EdgeTypename,
                typename std::conditional<useComposite, GraphUndirectedCompose<Graph1Type, MoreGraphs...>, void>::type>;
    };

    template<typename Graph1>
    struct GraphUndirectedCompose<Graph1> {
        using Graph = Graph1;
        static constexpr bool useComposite = Graph1::isOnNodeAdded or Graph1::isOnEdgeAdded;
    };

    template<typename Graph1, typename... MoreGraphs>
    inline auto onNodeAdded(const typename GraphUndirectedCompose<Graph1, MoreGraphs...>::NodeTypename &g,
                            typename GraphUndirectedCompose<Graph1, MoreGraphs...>::NodeTypename &p) {
        if constexpr(Graph1::isOnNodeAdded) {
            onNodeAdded(g, (typename Graph1::NodeTypename &) p);
        }

        if constexpr(sizeof...(MoreGraphs) > 1) {
            onNodeAdded<MoreGraphs...>(g, (typename GraphUndirectedCompose<MoreGraphs...>::Graph::NodeTypename &) p);
        }

        if constexpr(sizeof...(MoreGraphs) == 1) {
            using GraphType = typename GraphUndirectedCompose<MoreGraphs...>::Graph;
            if constexpr(GraphType::isOnNodeAdded) {
                onNodeAdded(g, (typename GraphType::NodeTypename &) p);
            }
        }
    }

    template<typename Graph1, typename... MoreGraphs>
    inline auto onEdgeAdded(const typename GraphUndirectedCompose<Graph1, MoreGraphs...>::NodeTypename &g,
                            typename GraphUndirectedCompose<Graph1, MoreGraphs...>::EdgeTypename &p) {
        if constexpr(Graph1::isOnNodeAdded) {
            onEdgeAdded(g, (typename Graph1::NodeTypename &) p);
        }

        if constexpr(sizeof...(MoreGraphs) > 1) {
            onEdgeAdded<MoreGraphs...>(g, (typename GraphUndirectedCompose<MoreGraphs...>::Graph::EdgeTypename &) p);
        }

        if constexpr(sizeof...(MoreGraphs) == 1) {
            using GraphType = typename GraphUndirectedCompose<MoreGraphs...>::Graph;
            if constexpr(GraphType::isOnNodeAdded) {
                onEdgeAdded(g, (typename GraphType::EdgeTypename &) p);
            }
        }
    }

}
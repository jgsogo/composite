#pragma once

#include <memory>
#include <vector>
#include "graph_undirected/visitor_graph.hpp"
#include "graph_undirected/visitor.hpp"
#include "graph_undirected/is_add_function.hpp"


namespace composite {

    /*
     * A 'GraphUndirected' is a data structure where its elements (nodes) are connected using edges. In an undirected
     * graph all connections are bidirectional
     * */
    template<typename Node, typename Edge, typename CompositeType = void>
    struct GraphUndirected {
        using NodeTypename = Node;
        using EdgeTypename = Edge;
        using CompositeTypename = CompositeType;

        static constexpr bool isCompose = not std::is_same_v<CompositeTypename, void>;
        static constexpr bool isOnNodeAdded = _impl::graph::is_onNodeAdded<NodeTypename>();
        static constexpr bool isOnEdgeAdded = _impl::graph::is_onEdgeAdded<NodeTypename, EdgeTypename>();

        class GraphNode;

        class GraphEdge;

        using DFSPreOrderVisitor = _impl::graph::Visitor<_impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge>>;
        using BFSVisitor = _impl::graph::Visitor<_impl::graph::BFSVisitorGraph<GraphNode, GraphEdge>>;

        class AddNodeVisitor;

        class GraphNode : public NodeTypename {
        public:
            using NodeTypename = GraphUndirected::NodeTypename;
        public:
            friend class _impl::graph::BFSVisitorGraph<GraphNode, GraphEdge>;

            friend class _impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge>;

            friend class AddNodeVisitor;


            template<typename ...Args>
            explicit GraphNode(Args... args) : NodeTypename(args...) {
                static int uniqueId = 0;
                _uniqueId = ++uniqueId;
            };

            template<typename ...ArgsEdge>
            std::shared_ptr<GraphEdge> connect(std::shared_ptr<GraphNode> item, ArgsEdge... argsEdge) {
                auto edge = std::make_shared<GraphEdge>(*this, *item, argsEdge...);
                _edges.push_back(std::make_pair(std::ref(*item), edge));
                item->_edges.push_back(std::make_pair(std::ref(*this), edge));

                if constexpr(isOnNodeAdded || isOnEdgeAdded) {
                    auto vPack = AddNodeVisitor{*this};
                    vPack.start(*this);
                }

                return edge;
            }

        protected:
            void accept(_impl::graph::VisitorGraph<GraphNode, GraphEdge> &v) {
                v.visit(*this);
            }

            /*
            template<typename TNode, typename TEdge, bool enable = isCompose>
            typename std::enable_if<enable>::type
            accept(_impl::Visitor <_impl::DFSVisitorGraph<TNode, TEdge>> &v) {
                _impl::VisitorWrapperCast <TNode, TEdge, GraphNode, GraphEdge, _impl::DFSVisitorGraph> wrapper{v};
                wrapper._onNode(*this);
            }

            template<typename TNode, typename TEdge, bool enable = isCompose>
            typename std::enable_if<enable>::type
            accept(_impl::Visitor <_impl::DFSVisitorGraph<TNode, TEdge>> &v) {
                _impl::VisitorWrapperCast <TNode, TEdge, GraphNode, GraphEdge, _impl::DFSVisitorGraph> wrapper{v};
                wrapper._onNode(*this);
            }
            */

        protected:
            int _uniqueId;
            std::vector<std::pair<std::reference_wrapper<GraphNode>, std::shared_ptr<GraphEdge>>> _edges;
        };


        class GraphEdge : public EdgeTypename {
        public:
            using EdgeTypename = GraphUndirected::EdgeTypename;
        public:
            friend class _impl::graph::BFSVisitorGraph<GraphNode, GraphEdge>;

            friend class _impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge>;

            template<typename ...Args>
            explicit GraphEdge(GraphNode &origin, GraphNode &target, Args... args)
                    : _origin(origin), _target(target), EdgeTypename(args...) {
                static int uniqueId = 0;
                _uniqueId = ++uniqueId;
            };

        protected:
            void accept(_impl::graph::VisitorGraph<GraphNode, GraphEdge> &v) {
                v.visit(*this);
            }

        protected:
            int _uniqueId;
            GraphNode &_origin;
            GraphNode &_target;
        };

        class AddNodeVisitor : public _impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge> {
        public:
            explicit AddNodeVisitor(GraphNode &node) : _node(node) {}

            template<typename...T>
            struct OnNodeAddedCaller;

            template<typename T1, typename... T2>
            struct OnNodeAddedCaller<T1, std::tuple<T2...>> {
                static auto call(const NodeTypename &g, NodeTypename &p) {
                    onNodeAdded<T1, T2...>(g, p);
                }
            };

            void visit(GraphNode &other) override {
                if constexpr(isOnNodeAdded) {
                    if (_node._uniqueId != other._uniqueId) {
                        if constexpr(isCompose) {
                            OnNodeAddedCaller<typename CompositeTypename::Graph1Type, typename CompositeTypename::MoreGraphsType>::call(_node,
                                                                                                                                        (NodeTypename &) other);
                        } else {
                            onNodeAdded(_node, other);
                        }
                    }
                }
            }

            template<typename...T>
            struct OnEdgeAddedCaller;

            template<typename T1, typename... T2>
            struct OnEdgeAddedCaller<T1, std::tuple<T2...>> {
                static auto call(const NodeTypename &g, EdgeTypename &p) {
                    onEdgeAdded<T1, T2...>(g, p);
                }
            };

            void visit(GraphEdge &other) override {
                if constexpr(isOnEdgeAdded) {
                    if constexpr(isCompose) {
                        OnEdgeAddedCaller<typename CompositeTypename::Graph1Type, typename CompositeTypename::MoreGraphsType>::call(_node,
                                                                                                                                    (EdgeTypename &) other);
                    } else {
                        onEdgeAdded(_node, other);
                    }
                }
            }

        protected:
            const GraphNode &_node;
        };

    };
}
#pragma once

#include <memory>
#include <vector>
#include "graph_undirected/visitor_graph.hpp"
#include "graph_undirected/visitor.hpp"


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
        //static constexpr bool isOnNodeAdded = _impl::is_onNodeAdded<NodeTypename>();

        class GraphNode;

        class GraphEdge;

        using DFSPreOrderVisitor = _impl::graph::Visitor<_impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge>>;
        using BFSVisitor = _impl::graph::Visitor<_impl::graph::BFSVisitorGraph<GraphNode, GraphEdge>>;


        class GraphNode : public NodeTypename {
        public:
            using NodeTypename = GraphUndirected::NodeTypename;
        public:
            friend class _impl::graph::BFSVisitorGraph<GraphNode, GraphEdge>;
            friend class _impl::graph::DFSPreOrderVisitorGraph<GraphNode, GraphEdge>;


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
                return edge;

                // TODO: On node connected
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
            /*
            template<bool enable = isOnNodeAdded>
            typename std::enable_if<enable || isCompose>::type
            _addChild(std::shared_ptr<TreeNode> item) {
                _children.push_back(item);
                auto vPack = AddNodeVisitor{*this};
                item->accept(vPack);
            }

            template<bool enable = isOnNodeAdded>
            typename std::enable_if<!enable && !isCompose>::type
            _addChild(std::shared_ptr<TreeNode> item) {
                _children.push_back(item);
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
    };
}
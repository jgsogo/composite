#pragma once

#include "visitor_wrapper_cast.hpp"

namespace composite::_impl::graph {

    template<typename GraphNodeT, typename GraphEdgeT, template<typename, typename> typename VisitorTrait>
    class Visitor : public VisitorTrait<GraphNodeT, GraphEdgeT> {
        using GraphNode = GraphNodeT;
        using GraphEdge = GraphEdgeT;
    public:
        Visitor() = default;

        virtual void visit(typename GraphNode::NodeTypename &) {}

        virtual void visit(typename GraphEdge::EdgeTypename &) {}

        virtual bool enterNode(typename GraphNode::NodeTypename &) { return true; }

        virtual void exitNode(typename GraphNode::NodeTypename &) {}

        virtual bool enterEdge(typename GraphEdge::EdgeTypename &) { return true; }

        virtual void exitEdge(typename GraphEdge::EdgeTypename &) {}

        template<typename TNode>
        typename std::enable_if_t<std::is_base_of_v<typename GraphNode::NodeTypename, TNode>>
        start(TNode &initNode) {
            VisitorWrapperCast<GraphNode, GraphEdge, TNode, typename TNode::GraphEdge, VisitorTrait> wrapper{*this};
            wrapper.start(initNode);
        }

    protected:
        void visit(GraphNode &p) final { this->visit(static_cast<typename GraphNode::NodeTypename &>(p)); }

        bool enterNode(GraphNode &g) final { return this->enterNode(static_cast<typename GraphNode::NodeTypename &>(g)); }

        void exitNode(GraphNode &g) final { this->exitNode(static_cast<typename GraphNode::NodeTypename &>(g)); }

        void visit(GraphEdge &p) final { this->visit(static_cast<typename GraphEdge::EdgeTypename &>(p)); }

        bool enterEdge(GraphEdge &g) final { return this->enterEdge(static_cast<typename GraphEdge::EdgeTypename &>(g)); }

        void exitEdge(GraphEdge &g) final { this->exitEdge(static_cast<typename GraphEdge::EdgeTypename &>(g)); }
    };

}

#pragma once

namespace composite::_impl::graph {
    template<typename TNode, typename TEdge, template<typename, typename> typename VisitorTrait>
    class Visitor;

    template<typename TNode, typename TEdge, typename ChildTNode, typename ChildTEdge, template<typename, typename> typename ImplVisitor>
    class VisitorWrapperCast : public ImplVisitor<ChildTNode, ChildTEdge> {
    public:
        explicit VisitorWrapperCast(Visitor<TNode, TEdge, ImplVisitor> &visitor) : visitor(visitor) {}

        void visit(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphNode &p) final {
            visitor.visit(static_cast<typename TNode::NodeTypename &>(p));
        }

        bool enterNode(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphNode &g) final {
            return visitor.enterNode(static_cast<typename TNode::NodeTypename &>(g));
        }

        void exitNode(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphNode &g) final {
            visitor.exitNode(static_cast<typename TNode::NodeTypename &>(g));
        }

        void visit(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphEdge &p) final {
            visitor.visit(static_cast<typename TEdge::EdgeTypename &>(p));
        }

        bool enterEdge(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphEdge &g) final {
            return visitor.enterEdge(static_cast<typename TEdge::EdgeTypename &>(g));
        }

        void exitEdge(typename ImplVisitor<ChildTNode, ChildTEdge>::GraphEdge &g) final {
            visitor.exitEdge(static_cast<typename TEdge::EdgeTypename &>(g));
        }

    protected:
        Visitor<TNode, TEdge, ImplVisitor> &visitor;
    };

}

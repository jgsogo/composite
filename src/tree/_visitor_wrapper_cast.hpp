#pragma once

#include "_visitor_tree.hpp"
#include "visitor.hpp"

namespace composite::_impl {

    template<typename TNode, typename ChildTNode, template<typename> typename ImplVisitor>
    class VisitorWrapperCast : public ImplVisitor<ChildTNode> {
    public:
        explicit VisitorWrapperCast(Visitor<TNode, ImplVisitor> &visitor) : visitor(visitor) {}

        void visit(typename ImplVisitor<ChildTNode>::TreeNode &p) final {
            visitor.visit(static_cast<typename TNode::NodeTypename &>(p));
        }

        bool enterNode(typename ImplVisitor<ChildTNode>::TreeNode &g) final {
            return visitor.enterNode(static_cast<typename TNode::NodeTypename &>(g));
        }

        void exitNode(typename ImplVisitor<ChildTNode>::TreeNode &g) final {
            visitor.exitNode(static_cast<typename TNode::NodeTypename &>(g));
        }

    protected:
        Visitor<TNode, ImplVisitor> &visitor;
    };

}

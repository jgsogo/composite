#pragma once

#include "_visitor_tree.hpp"
#include "visitor.hpp"

namespace composite::_impl {

    template<typename TNode, typename ChildTNode, template<typename> typename ImplVisitor>
    class VisitorWrapperCast : public ImplVisitor<ChildTNode> {
    public:
        explicit VisitorWrapperCast(Visitor<ImplVisitor<TNode>> &visitor) : visitor(visitor) {}

        void visit(typename ImplVisitor<ChildTNode>::TreeNode &p) final {
            visitor.visit(static_cast<typename TNode::NodeTypename &>(p));
        }

        bool enterGroup(typename ImplVisitor<ChildTNode>::TreeNode &g) final {
            return visitor.enterGroup(static_cast<typename TNode::NodeTypename &>(g));
        }

        void exitGroup(typename ImplVisitor<ChildTNode>::TreeNode &g) final {
            visitor.exitGroup(static_cast<typename TNode::NodeTypename &>(g));
        }

    protected:
        Visitor<ImplVisitor<TNode>> &visitor;
    };

}

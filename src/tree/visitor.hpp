#pragma once

#include "visitor_wrapper_cast.hpp"

namespace composite::_impl::tree {

    template<typename TreeNodeT, template<typename> typename VisitorTrait>
    class Visitor : public VisitorTrait<TreeNodeT> {
        using TreeNode = typename VisitorTrait<TreeNodeT>::TreeNode;
    public:
        Visitor() = default;

        virtual void visit(typename TreeNode::NodeTypename &) {}

        virtual bool enterNode(typename TreeNode::NodeTypename &) { return true; }

        virtual void exitNode(typename TreeNode::NodeTypename &) {}

        template<typename TNode>
        typename std::enable_if_t<std::is_base_of_v<typename TreeNode ::NodeTypename, TNode>>
        start(TNode &initNode) {
            VisitorWrapperCast<TreeNode , TNode, VisitorTrait> wrapper{*this};
            wrapper.start(initNode);
        }

    protected:
        void visit(TreeNode &p) final { this->visit(static_cast<typename TreeNode::NodeTypename &>(p)); }

        bool enterNode(TreeNode &g) final { return this->enterNode(static_cast<typename TreeNode::NodeTypename &>(g)); }

        void exitNode(TreeNode &g) final { this->exitNode(static_cast<typename TreeNode::NodeTypename &>(g)); }
    };

}

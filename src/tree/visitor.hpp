#pragma once

namespace composite::_impl {

    template<typename TreeNodeT, template<typename> typename VisitorTrait>
    class Visitor : public VisitorTrait<TreeNodeT> {
        using TreeNode = typename VisitorTrait<TreeNodeT>::TreeNode;
    public:
        Visitor() = default;

        virtual void visit(typename TreeNode::NodeTypename &) {}

        virtual bool enterNode(typename TreeNode::NodeTypename &) { return true; }

        virtual void exitNode(typename TreeNode::NodeTypename &) {}

    protected:
        void visit(TreeNode &p) final { this->visit(static_cast<typename TreeNode::NodeTypename &>(p)); }

        bool enterNode(TreeNode &g) final { return this->enterNode(static_cast<typename TreeNode::NodeTypename &>(g)); }

        void exitNode(TreeNode &g) final { this->exitNode(static_cast<typename TreeNode::NodeTypename &>(g)); }
    };

}

#pragma once

namespace composite::_impl {

    template<typename TreeNodeT>
    class VisitorTree {
    public:
        using TreeNode = TreeNodeT;

    public:
        VisitorTree() = default;

        virtual void visit(TreeNode &) {}

        virtual bool enterNode(TreeNode &) { return true; }

        virtual void exitNode(TreeNode &) {}

        void _visitChildren(TreeNode &node) {
            for (auto &it: node._children) {
                it->accept(*this);
            }
        }

        virtual void _onNode(TreeNode &node) = 0;
    };

    template<typename TreeNodeT>
    class DFSVisitorTree : public VisitorTree<TreeNodeT> {
    public:
        DFSVisitorTree() = default;

        void _onNode(TreeNodeT &node) final {
            if (this->enterNode(node)) {
                this->_visitChildren(node);
                this->visit(node);
                this->exitNode(node);
            }
        }
    };

    template<typename TreeNodeT>
    class BFSVisitorTree : public VisitorTree<TreeNodeT> {
    public:
        BFSVisitorTree() = default;

        void _onNode(TreeNodeT &node) final {
            if (this->enterNode(node)) {
                this->visit(node);
                this->_visitChildren(node);
                this->exitNode(node);
            }
        }
    };


}

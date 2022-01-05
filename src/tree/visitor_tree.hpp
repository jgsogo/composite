#pragma once

#include <set>
#include <stack>
#include <queue>

namespace composite::_impl::tree {

    template<typename TreeNodeT>
    class VisitorTree {
    public:
        using TreeNode = TreeNodeT;

    public:
        VisitorTree() = default;

        virtual void visit(TreeNode &) {}

        virtual bool enterNode(TreeNode &) { return true; }

        virtual void exitNode(TreeNode &) {}

        virtual void start(TreeNode &node) = 0;
    };

    template<typename TreeNodeT>
    class DFSPreOrderVisitorTree : public VisitorTree<TreeNodeT> {
    public:
        DFSPreOrderVisitorTree() = default;

        void start(TreeNodeT &initNode) override {
            /* RECURSIVE implementation, so 'exitNode' is called AFTER all its childrent have been processed */
            std::set<int> visitedNodes;
            //std::stack<std::reference_wrapper<TreeNodeT>> stack;

            visitedNodes.insert(initNode._uniqueId);
            this->enterNode(initNode);
            //stack.push(initNode);

            std::function<void(TreeNodeT &item)> onNode = [&](TreeNodeT &item) {
                this->visit(item);
                for (auto &it: item._children) {
                    auto[_, inserted] = visitedNodes.insert(it->_uniqueId);
                    if (inserted && this->enterNode(item)) {  // TODO: Rename enterNode to discoverNode
                        onNode(*it);
                        //stack.push(std::ref(*it));
                    }
                }
                this->exitNode(item);  // TODO: Rename exitNode to finishNode
            };

            onNode(initNode);
            /*
            while (!stack.empty()) {
                auto &node = stack.top();
                stack.pop();
                onNode(node);
            }
             */
        }
    };

    template<typename TreeNodeT>
    class BFSVisitorTree : public VisitorTree<TreeNodeT> {
    public:
        BFSVisitorTree() = default;

        void start(TreeNodeT &initNode) override {
            std::set<int> visitedNodes;
            std::queue<std::reference_wrapper<TreeNodeT>> queue;

            visitedNodes.insert(initNode._uniqueId);
            this->enterNode(initNode);
            queue.push(initNode);

            auto onNode = [&](TreeNodeT &item) {
                this->visit(item);
                for (auto &it: item._children) {
                    auto[_, inserted] = visitedNodes.insert(it->_uniqueId);
                    if (inserted && this->enterNode(item)) {  // TODO: Rename enterNode to discoverNode
                        queue.push(std::ref(*it));
                    }
                }
                this->exitNode(item);  // TODO: Rename exitNode to finishNode
            };

            while (!queue.empty()) {
                auto &node = queue.front();
                queue.pop();
                onNode(node);
            }
        }

    };


}

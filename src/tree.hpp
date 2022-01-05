#pragma once

#include "tree/_is_add_function.hpp"
#include "tree/_visitor_tree.hpp"
#include "tree/_visitor_wrapper_cast.hpp"
#include "tree/visitor.hpp"

namespace composite {

    /*
     * A 'Tree' is a data structure where its elements can have zero or more children, and every
     * element except the 'root' have one parent.
     * */
    template<typename Node, typename CompositeType = void>
    struct Tree {
        using NodeTypename = Node;
        using CompositeTypename = CompositeType;

        static constexpr bool isCompose = not std::is_same_v<CompositeTypename, void>;
        static constexpr bool isOnNodeAdded = _impl::is_onNodeAdded<NodeTypename>();

        class TreeNode;

        using DFSVisitor = _impl::Visitor<TreeNode, _impl::DFSVisitorTree>;
        using BFSVisitor = _impl::Visitor<TreeNode, _impl::BFSVisitorTree>;

        class TreeNode : public NodeTypename {
        public:
            using NodeTypename = Tree::NodeTypename;
        public:
            friend class _impl::VisitorTree<TreeNode>;

            template<typename ...Args>
            explicit TreeNode(Args... args) : NodeTypename(args...) {};

            void addChild(std::shared_ptr<TreeNode> item) {
                _addChild(item);
            }

            void accept(_impl::VisitorTree<TreeNode> &v) {
                v._onNode(*this);
            }

            template<typename TNode, bool enable = isCompose>
            typename std::enable_if<enable>::type
            accept(_impl::Visitor<TNode, _impl::DFSVisitorTree> &v) {
                _impl::VisitorWrapperCast<TNode, TreeNode, _impl::DFSVisitorTree> wrapper{v};
                wrapper._onNode(*this);
            }

            template<typename TNode, bool enable = isCompose>
            typename std::enable_if<enable>::type
            accept(_impl::Visitor<TNode, _impl::BFSVisitorTree> &v) {
                _impl::VisitorWrapperCast<TNode, TreeNode, _impl::BFSVisitorTree> wrapper{v};
                wrapper._onNode(*this);
            }

        protected:
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

        protected:
            std::vector<std::shared_ptr<TreeNode>> _children;
        };


        class AddNodeVisitor : public _impl::DFSVisitorTree<TreeNode> {
        public:
            explicit AddNodeVisitor(TreeNode &gr) {
                _chain.template emplace_back(gr);
            }

            AddNodeVisitor(std::vector<std::reference_wrapper<NodeTypename>> &v, TreeNode &gr) : _chain(v) {
                _chain.template emplace_back(gr);
            }

            template<typename...T>
            struct OnNodeAddedCaller;

            template<typename T1, typename... T2>
            struct OnNodeAddedCaller<T1, std::tuple<T2...>> {
                static auto call(const std::vector<std::reference_wrapper<NodeTypename>> &g, NodeTypename &p) {
                    onNodeAdded<T1, T2...>(g, p);
                }
            };

            bool enterNode(TreeNode &node) override {
                if constexpr(isCompose) {
                    OnNodeAddedCaller<typename CompositeTypename::Tree1Type, typename CompositeTypename::MoreTreesType>::call(_chain, (NodeTypename &) node);
                } else {
                    onNodeAdded(_chain, (NodeTypename &) node);
                }

                auto recurse = AddNodeVisitor{_chain, node};
                recurse._visitChildren(node);
                return false;
            }

        protected:
            std::vector<std::reference_wrapper<NodeTypename>> _chain;
        };

    };
}


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
            friend class _impl::BFSVisitorTree<TreeNode>;

            friend class _impl::DFSVisitorTree<TreeNode>;

            template<typename ...Args>
            explicit TreeNode(Args... args) : NodeTypename(args...) {
                static int uniqueId = 0;
                _uniqueId = ++uniqueId;
            };

            void addChild(std::shared_ptr<TreeNode> item) {
                _addChild(item);
            }

        protected:
            void accept(_impl::VisitorTree<TreeNode> &v) {
                v.visit(*this);
            }

            template<bool enable = isOnNodeAdded>
            typename std::enable_if<enable || isCompose>::type
            _addChild(std::shared_ptr<TreeNode> item) {
                _children.push_back(item);
                auto vPack = AddNodeVisitor{*this};
                vPack.start(*item);
            }

            template<bool enable = isOnNodeAdded>
            typename std::enable_if<!enable && !isCompose>::type
            _addChild(std::shared_ptr<TreeNode> item) {
                _children.push_back(item);
            }

        protected:
            int _uniqueId = -1;
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

            void visit(TreeNode &node) override {
                if constexpr(isCompose) {
                    OnNodeAddedCaller<typename CompositeTypename::Tree1Type, typename CompositeTypename::MoreTreesType>::call(_chain, (NodeTypename &) node);
                } else {
                    onNodeAdded(_chain, (NodeTypename &) node);
                }
                _chain.push_back(node);
                //auto recurse = AddNodeVisitor{_chain, node};
                //recurse.start(node);
                //return false;
            }

            void exitNode(TreeNode& node) override {
                _chain.pop_back();
            }

        protected:
            std::vector<std::reference_wrapper<NodeTypename>> _chain;
        };

    };
}


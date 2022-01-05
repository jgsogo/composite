
#include <string>
#include "catch2/catch.hpp"
#include "tree.hpp"

namespace {

    struct NodeId {
        explicit NodeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    using IdNode = composite::Tree<NodeId>;
}


TEST_CASE("test_tree/test_visitor | Visitor DFS", "[visitor]") {
    auto root = std::make_shared<IdNode::TreeNode>("root");
    auto node1 = std::make_shared<IdNode::TreeNode>("node1");
    root->addChild(node1);

    auto node2 = std::make_shared<IdNode::TreeNode>("node2");
    auto node3 = std::make_shared<IdNode::TreeNode>("node3");
    node2->addChild(node3);
    node1->addChild(node2);

    class Visitor : public IdNode::DFSVisitor {
    public:
        void visit(NodeId &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*root);

    REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
}

TEST_CASE("test_tree/test_visitor | Visitor BFS", "[visitor]") {
    auto root = std::make_shared<IdNode::TreeNode>("root");
    auto node1 = std::make_shared<IdNode::TreeNode>("node1");
    root->addChild(node1);

    auto node2 = std::make_shared<IdNode::TreeNode>("node2");
    auto node3 = std::make_shared<IdNode::TreeNode>("node3");
    node2->addChild(node3);
    root->addChild(node2);

    class Visitor : public IdNode::BFSVisitor {
    public:
        void visit(NodeId &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*root);

    REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
}

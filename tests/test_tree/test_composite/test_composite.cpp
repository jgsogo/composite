
#include <string>
#include <catch2/catch.hpp>
#include "tree_composite.hpp"

namespace {

    struct NodeId {
        NodeId(std::string_view id) : id(id) {}

        const std::string id;
    };


    struct NodeIdNum {
        NodeIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int depth = 0;
    };

    inline void onNodeAdded(const std::vector<std::reference_wrapper<NodeIdNum>> &gs, NodeIdNum &p) {
        p.depth = gs.size();
    }

    using IDTree = composite::Tree<NodeId>;
    using IDNumTree = composite::Tree<NodeIdNum>;
    using CompositeTree = composite::TreeCompose<IDTree, IDNumTree>::Tree;
}

TEST_CASE("test_tree/test_composite | Assert traits", "[test_composite]") {
    REQUIRE(!IDTree::isCompose);
    REQUIRE(!IDTree::isOnNodeAdded);
    REQUIRE(!IDNumTree::isCompose);
    REQUIRE(IDNumTree::isOnNodeAdded);
    REQUIRE(CompositeTree::isCompose);
}

TEST_CASE("test_tree/test_composite | Node create", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 23);
    REQUIRE(root->id == "root");
    REQUIRE(root->idNum == 23);
    REQUIRE(root->depth == 0);
}

TEST_CASE("test_tree/test_composite | onNodeAdded function", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 23);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 42);
    root->addChild(node1);

    REQUIRE(root->id == "root");
    REQUIRE(root->idNum == 23);
    REQUIRE(root->depth == 0);

    REQUIRE(node1->id == "node1");
    REQUIRE(node1->idNum == 42);
    REQUIRE(node1->depth == 1);
}

TEST_CASE("test_tree/test_composite | Visitor composite", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 0);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 1);
    root->addChild(node1);

    auto node2 = std::make_shared<CompositeTree::TreeNode>("node2", 2);
    auto node3 = std::make_shared<CompositeTree::TreeNode>("node3", 3);
    node2->addChild(node3);
    root->addChild(node2);


    class Visitor : public CompositeTree::DFSVisitor {
    public:
        void visit(CompositeTree::NodeTypename &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*root);

    REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
}


TEST_CASE("test_tree/test_composite | Visitor first trait", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 0);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 1);
    root->addChild(node1);

    auto node2 = std::make_shared<CompositeTree::TreeNode>("node2", 2);
    auto node3 = std::make_shared<CompositeTree::TreeNode>("node3", 3);
    node2->addChild(node3);
    root->addChild(node2);

    {
        class Visitor : public IDTree::DFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDTree::NodeTypename &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;
        visitor.start(*root);

        REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
    }
    {
        class Visitor : public IDTree::BFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDTree::NodeTypename &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;
        visitor.start(*root);

        REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
    }
}

TEST_CASE("test_tree/test_composite | Visitor second trait", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 0);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 1);
    root->addChild(node1);

    auto node2 = std::make_shared<CompositeTree::TreeNode>("node2", 2);
    auto node3 = std::make_shared<CompositeTree::TreeNode>("node3", 3);
    node2->addChild(node3);
    root->addChild(node2);


    {
        class Visitor : public IDNumTree::DFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDNumTree::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;
        visitor.start(*root);

        REQUIRE(visitor.ids == std::vector<int>{0, 1, 2, 3});
    }
    {
        class Visitor : public IDNumTree::BFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDNumTree::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;
        visitor.start(*root);

        REQUIRE(visitor.ids == std::vector<int>{0, 1, 2, 3});
    }
}

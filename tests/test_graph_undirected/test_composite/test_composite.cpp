
#include <string>
#include "catch2/catch.hpp"
#include "graph_undirected_composite.hpp"

namespace {

    struct NodeId {
        NodeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    struct EdgeId {
        EdgeId(std::string_view id) : id(id) {}

        const std::string id;
    };


    struct NodeIdNum {
        NodeIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int family = -1;
    };

    struct EdgeIdNum {
        EdgeIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int family = -1;
    };


    inline void onNodeAdded(const NodeIdNum &gs, NodeIdNum &p) {
        p.family = gs.family;
    }

    using IDGraph = composite::GraphUndirected<NodeId, EdgeId>;
    using IDNumGraph = composite::GraphUndirected<NodeIdNum, EdgeIdNum>;
    using CompositeGraph = composite::GraphUndirectedCompose<IDGraph, IDNumGraph>::Graph;
}

TEST_CASE("test_graph_undirected/test_composite | Assert traits", "[test_composite]") {
    REQUIRE(!IDGraph::isCompose);
    REQUIRE(!IDGraph::isOnNodeAdded);
    REQUIRE(!IDGraph::isOnEdgeAdded);
    REQUIRE(!IDNumGraph::isCompose);
    REQUIRE(IDNumGraph::isOnNodeAdded);
    REQUIRE(!IDNumGraph::isOnEdgeAdded);
    REQUIRE(CompositeGraph::isCompose);
}


TEST_CASE("test_graph_undirected/test_composite | Node create", "[test_composite]") {
    auto root = std::make_shared<CompositeGraph::GraphNode>("root", 23);
    REQUIRE(root->id == "root");
    REQUIRE(root->idNum == 23);
    REQUIRE(root->family == -1);
}

TEST_CASE("test_graph_undirected/test_composite | onNodeAdded function", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1", 23);
    n1->family = 23;
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2", 42);
    auto e12 = n1->connect(n2, "e12", 11);

    REQUIRE(n1->id == "n1");
    REQUIRE(n1->idNum == 23);
    REQUIRE(n1->family == 23);

    REQUIRE(n2->id == "n2");
    REQUIRE(n2->idNum == 42);
    REQUIRE(n2->family == 23);

    REQUIRE(e12->id == "e12");
    REQUIRE(e12->idNum == 11);
    REQUIRE(e12->family == -1);  // The family of edges is not updated, there is no 'onEdgeAdded' function
}

/*
TEST_CASE("test_graph_undirected/test_composite | Visitor composite", "[test_composite]") {
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
    root->accept(visitor);

    REQUIRE(visitor.ids == std::vector<std::string>{"node1", "node3", "node2", "root"});
}


TEST_CASE("test_graph_undirected/test_composite | Visitor first trait", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 0);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 1);
    root->addChild(node1);

    auto node2 = std::make_shared<CompositeTree::TreeNode>("node2", 2);
    auto node3 = std::make_shared<CompositeTree::TreeNode>("node3", 3);
    node2->addChild(node3);
    root->addChild(node2);

    {
        class Visitor : public IDTree::DFSVisitor {
        public:
            void visit(IDTree::NodeTypename &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;

        root->accept(visitor);

        REQUIRE(visitor.ids == std::vector<std::string>{"node1", "node3", "node2", "root"});
    }
    {
        class Visitor : public IDTree::BFSVisitor {
        public:
            void visit(IDTree::NodeTypename &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;

        root->accept(visitor);

        REQUIRE(visitor.ids == std::vector<std::string>{"root", "node1", "node2", "node3"});
    }
}

TEST_CASE("test_graph_undirected/test_composite | Visitor second trait", "[test_composite]") {
    auto root = std::make_shared<CompositeTree::TreeNode>("root", 0);
    auto node1 = std::make_shared<CompositeTree::TreeNode>("node1", 1);
    root->addChild(node1);

    auto node2 = std::make_shared<CompositeTree::TreeNode>("node2", 2);
    auto node3 = std::make_shared<CompositeTree::TreeNode>("node3", 3);
    node2->addChild(node3);
    root->addChild(node2);


    {
        class Visitor : public IDNumTree::DFSVisitor {
        public:
            void visit(IDNumTree::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;

        root->accept(visitor);

        REQUIRE(visitor.ids == std::vector<int>{1, 3, 2, 0});
    }
    {
        class Visitor : public IDNumTree::BFSVisitor {
        public:
            void visit(IDNumTree::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;

        root->accept(visitor);

        REQUIRE(visitor.ids == std::vector<int>{0, 1, 2, 3});
    }
}
 */

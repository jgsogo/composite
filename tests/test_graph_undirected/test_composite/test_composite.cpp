
#include <string>
#include "catch2/catch.hpp"
#include "graph_undirected/graph_undirected_composite.hpp"

namespace {

    struct NodeId {
        explicit NodeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    struct EdgeId {
        explicit EdgeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    struct NodeIdNum {
        explicit NodeIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int family = -1;
    };

    struct EdgeIdNum {
        explicit EdgeIdNum(int idNum) : idNum(idNum) {}

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


TEST_CASE("test_graph_undirected/test_composite | Visitor composite", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1", 1);
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2", 2);
    auto n3 = std::make_shared<CompositeGraph::GraphNode>("n3", 3);
    auto n4 = std::make_shared<CompositeGraph::GraphNode>("n4", 4);

    n1->connect(n2, "n1-n2", 12);
    n1->connect(n3, "n1-n3", 13);

    n2->connect(n4, "n2-n4", 24);
    n3->connect(n4, "n3-n4", 34);


    class Visitor : public CompositeGraph::DFSPreOrderVisitor {
    public:
        void visit(CompositeGraph::NodeTypename &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*n1);

    REQUIRE(visitor.ids == std::vector<std::string>{"n1", "n2", "n4", "n3"});
}


TEST_CASE("test_graph_undirected/test_composite | Visitor first trait", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1", 1);
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2", 2);
    auto n3 = std::make_shared<CompositeGraph::GraphNode>("n3", 3);
    auto n4 = std::make_shared<CompositeGraph::GraphNode>("n4", 4);

    n1->connect(n2, "n1-n2", 12);
    n1->connect(n3, "n1-n3", 13);

    n2->connect(n4, "n2-n4", 24);
    n3->connect(n4, "n3-n4", 34);

    {
        class Visitor : public IDGraph::DFSPreOrderVisitor {
        public:
            void visit(NodeId &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;
        visitor.start(*n1);

        REQUIRE(visitor.ids == std::vector<std::string>{"n1", "n2", "n4", "n3"});
    }
    {
        class Visitor : public IDGraph::BFSVisitor {
        public:
            void visit(IDGraph::NodeTypename &p) override {
                ids.push_back(p.id);
            }

        public:
            std::vector<std::string> ids;
        };
        Visitor visitor;
        visitor.start(*n1);

        REQUIRE(visitor.ids == std::vector<std::string>{"n1", "n2", "n3", "n4"});
    }
}

TEST_CASE("test_graph_undirected/test_composite | Visitor second trait", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1", 1);
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2", 2);
    auto n3 = std::make_shared<CompositeGraph::GraphNode>("n3", 3);
    auto n4 = std::make_shared<CompositeGraph::GraphNode>("n4", 4);

    n1->connect(n2, "n1-n2", 12);
    n1->connect(n3, "n1-n3", 13);

    n2->connect(n4, "n2-n4", 24);
    n3->connect(n4, "n3-n4", 34);


    {
        class Visitor : public IDNumGraph::DFSPreOrderVisitor {
        public:
            void visit(IDNumGraph::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;
        visitor.start(*n1);

        REQUIRE(visitor.ids == std::vector<int>{1, 2, 4, 3});
    }
    {
        class Visitor : public IDNumGraph::BFSVisitor {
        public:
            void visit(IDNumGraph::NodeTypename &p) override {
                ids.push_back(p.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;
        visitor.start(*n1);

        REQUIRE(visitor.ids == std::vector<int>{1, 2, 3, 4});
    }
}

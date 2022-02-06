
#include <string>
#include <catch2/catch.hpp>
#include "graph_undirected.hpp"

namespace {

    struct NodeId {
        explicit NodeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    struct EdgeId {
        explicit EdgeId(std::string_view id) : id(id) {}

        const std::string id;
    };

    using IdGraph = composite::GraphUndirected<NodeId, EdgeId>;
}


TEST_CASE("test_graph_undirected/test_visitor | Visitor DFS pre-order", "[visitor]") {
    auto n1 = std::make_shared<IdGraph::GraphNode>("n1");
    auto n2 = std::make_shared<IdGraph::GraphNode>("n2");
    auto n3 = std::make_shared<IdGraph::GraphNode>("n3");
    auto n4 = std::make_shared<IdGraph::GraphNode>("n4");

    n1->connect(n2, "n1-n2");
    n1->connect(n3, "n1-n3");

    n2->connect(n4, "n2-n4");
    n3->connect(n4, "n3-n4");


    class Visitor : public IdGraph::DFSPreOrderVisitor {
    public:
        void visit(NodeId &p) override {
            ids.push_back(p.id);
        }

        void visit(EdgeId &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };

    // Because it is indirected, all nodes are visited regardless of the starting one
    Visitor visitor;
    visitor.start(*n1);
    REQUIRE(visitor.ids == std::vector<std::string>{"n1", "n1-n2", "n2", "n2-n4", "n4", "n3-n4", "n3", "n1-n3"});

    visitor.ids.clear();
    visitor.start(*n2);
    REQUIRE(visitor.ids == std::vector<std::string>{"n2", "n1-n2", "n1", "n1-n3", "n3", "n3-n4", "n4", "n2-n4"});
}


TEST_CASE("test_graph_undirected/test_visitor | Visitor BFS", "[visitor]") {
    auto n1 = std::make_shared<IdGraph::GraphNode>("n1");
    auto n2 = std::make_shared<IdGraph::GraphNode>("n2");
    auto n3 = std::make_shared<IdGraph::GraphNode>("n3");
    auto n4 = std::make_shared<IdGraph::GraphNode>("n4");

    n1->connect(n2, "n1-n2");
    n1->connect(n3, "n1-n3");

    n2->connect(n4, "n2-n4");
    n3->connect(n4, "n3-n4");

    class Visitor : public IdGraph::BFSVisitor {
    public:
        void visit(NodeId &p) override {
            ids.push_back(p.id);
        }

        void visit(EdgeId &p) override {
            ids.push_back(p.id);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;

    visitor.start(*n1);
    REQUIRE(visitor.ids == std::vector<std::string>{"n1", "n1-n2", "n2", "n1-n3", "n3", "n2-n4", "n4", "n3-n4"});

    visitor.ids.clear();
    visitor.start(*n2);
    REQUIRE(visitor.ids == std::vector<std::string>{"n2", "n1-n2", "n1", "n2-n4", "n4", "n1-n3", "n3", "n3-n4"});
}

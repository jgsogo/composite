
#include <string>
#include "catch2/catch.hpp"
#include "graph_undirected_composite.hpp"

namespace {

    struct Data1Node {
        Data1Node(std::string_view id) : data1(id) {}

        const std::string data1;
        int family1 = -1;
    };

    struct Data1Edge {
        Data1Edge(std::string_view id) : data1(id) {}

        const std::string data1;
        int family1 = -1;
    };

    struct Data2Node {
        Data2Node(std::string_view id) : data2(id) {}

        const std::string data2;
        int family2 = -1;
    };

    struct Data2Edge {
        Data2Edge(std::string_view id) : data2(id) {}

        const std::string data2;
        int family2 = -1;
    };

    struct Data3Node {
        Data3Node(std::string_view id) : data3(id) {}

        const std::string data3;
        int family3 = -1;
    };

    struct Data3Edge {
        Data3Edge(std::string_view id) : data3(id) {}

        const std::string data3;
        int family3 = -1;
    };

    using Trait1 = composite::GraphUndirected<Data1Node, Data1Edge>;
    using Trait2 = composite::GraphUndirected<Data2Node, Data2Edge>;
    using Trait3 = composite::GraphUndirected<Data3Node, Data3Edge>;

    inline void onNodeAdded(const Data1Node &gs, Data1Node &p) {
        p.family1 = gs.family1;
    }

    inline void onNodeAdded(const Data3Node &gs, Data3Node &p) {
        p.family3 = gs.family3;
    }

    using CompositeGraph = composite::GraphUndirectedCompose<Trait1, Trait2, Trait3>::Graph;
}

TEST_CASE("test_graph_undirected/test_composite | TraitGroup create composite3", "[test_composite]") {
    auto group = std::make_shared<CompositeGraph::GraphNode>("g1-1", "g1-2", "g1-3");
    REQUIRE(group->data1 == "g1-1");
    REQUIRE(group->data2 == "g1-2");
    REQUIRE(group->data3 == "g1-3");
}

TEST_CASE("test_graph_undirected/test_composite | Add part to composite3", "[test_composite]") {
    auto group = std::make_shared<CompositeGraph::GraphNode>("g1-1", "g1-2", "g1-3");
    auto part = std::make_shared<CompositeGraph::GraphNode>("p1-1", "p1-2", "p1-3");
    group->connect(part, "e1", "e2", "e3");

    REQUIRE(group->data1 == "g1-1");
    REQUIRE(group->family1 == -1);
    REQUIRE(group->data2 == "g1-2");
    REQUIRE(group->family2 == -1);
    REQUIRE(group->data3 == "g1-3");
    REQUIRE(group->family3 == -1);

    REQUIRE(part->data1 == "p1-1");
    REQUIRE(part->family1 == -1);
    REQUIRE(part->data2 == "p1-2");
    REQUIRE(part->family2 == -1);
    REQUIRE(part->data3 == "p1-3");
    REQUIRE(part->family3 == -1);
}


TEST_CASE("test_graph_undirected/test_composite | Visitor composite3", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1-1", "n1-2", "n1-3");
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2-1", "n2-2", "n2-3");
    auto n3 = std::make_shared<CompositeGraph::GraphNode>("n3-1", "n3-2", "n3-3");
    auto n4 = std::make_shared<CompositeGraph::GraphNode>("n4-1", "n4-2", "n4-3");

    n1->connect(n2, "n1-n2-1", "n1-n2-2", "n1-n2-3");
    n1->connect(n3, "n1-n3-1", "n1-n3-2", "n1-n3-3");

    n2->connect(n4, "n2-n4-1", "n2-n4-2", "n2-n4-3");
    n3->connect(n4, "n3-n4-1", "n3-n4-2", "n3-n4-3");


    class Visitor : public CompositeGraph::DFSPreOrderVisitor {
    public:
        void visit(CompositeGraph::NodeTypename &p) override {
            ids.push_back(p.data1);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*n1);

    REQUIRE(visitor.ids == std::vector<std::string>{"n1-1", "n2-1", "n4-1", "n3-1"});
}


TEST_CASE("test_graph_undirected | Visitor first trait", "[test_composite]") {
    auto n1 = std::make_shared<CompositeGraph::GraphNode>("n1-1", "n1-2", "n1-3");
    auto n2 = std::make_shared<CompositeGraph::GraphNode>("n2-1", "n2-2", "n2-3");
    auto n3 = std::make_shared<CompositeGraph::GraphNode>("n3-1", "n3-2", "n3-3");
    auto n4 = std::make_shared<CompositeGraph::GraphNode>("n4-1", "n4-2", "n4-3");

    n1->connect(n2, "n1-n2-1", "n1-n2-2", "n1-n2-3");
    n1->connect(n3, "n1-n3-1", "n1-n3-2", "n1-n3-3");

    n2->connect(n4, "n2-n4-1", "n2-n4-2", "n2-n4-3");
    n3->connect(n4, "n3-n4-1", "n3-n4-2", "n3-n4-3");


    class Visitor : public Trait2::DFSPreOrderVisitor {
    public:
        void visit(Trait2::NodeTypename &p) override {
            ids.push_back(p.data2);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    visitor.start(*n1);

    REQUIRE(visitor.ids == std::vector<std::string>{"n1-2", "n2-2", "n4-2", "n3-2"});
}


#include <string>
#include "catch2/catch.hpp"
#include "graph_undirected.hpp"

namespace {

    struct NodeId {
        NodeId(std::string_view id) : id(id) {}

        const std::string id;
        int family = -1;
    };

    struct EdgeId {
        EdgeId(std::string_view id) : id(id) {}

        const std::string id;
        int family = -1;
    };

    using IdGraphUndirected = composite::GraphUndirected<NodeId, EdgeId>;

    // TALK: Argument-dependent-lookup (ADL) and why this function has to be written here: https://stackoverflow.com/questions/55878310/why-do-type-traits-not-work-with-types-in-namespace-scope
    inline void onNodeAdded(const NodeId &existing, NodeId &newNode) {
        newNode.family = existing.family;
    }

    inline void onEdgeAdded(const NodeId &existing, EdgeId &newEdge) {
        newEdge.family = existing.family;
    }
}


TEST_CASE("test_graph_undirected/test_simple | onNodeAdded function", "[trait_addpart]") {
    REQUIRE(composite::_impl::graph::is_onNodeAdded<NodeId>());
}

TEST_CASE("test_graph_undirected/test_simple | onEdgeAdded function", "[trait_addpart]") {
    REQUIRE(composite::_impl::graph::is_onEdgeAdded<NodeId, EdgeId>());
}

TEST_CASE("test_graph_undirected/test_simple | onNodeAdded function is used", "[trait_addpart]") {
    auto n1 = std::make_shared<IdGraphUndirected::GraphNode>("n1");
    auto n2 = std::make_shared<IdGraphUndirected::GraphNode>("n2");
    auto n3 = std::make_shared<IdGraphUndirected::GraphNode>("n3");
    auto n4 = std::make_shared<IdGraphUndirected::GraphNode>("n4");
    REQUIRE(n1->family == -1);
    REQUIRE(n2->family == -1);
    REQUIRE(n3->family == -1);
    REQUIRE(n4->family == -1);

    n1->family = 1;
    auto e12 = n1->connect(n2, "n1-n2");
    REQUIRE(n2->family == 1);
    REQUIRE(e12->family == 1);

    n3->family = 3;
    auto e34 = n3->connect(n4, "n3-n4");
    REQUIRE(n4->family == 3);
    REQUIRE(e34->family == 3);

    auto e24 = n2->connect(n4, "n2-n4");
    REQUIRE(n3->family == 1);
    REQUIRE(n4->family == 1);
    REQUIRE(e34->family == 1);
    REQUIRE(e24->family == 1);

    auto e13 = n1->connect(n3, "n1-n3");
    REQUIRE(e13->family == 1);
}

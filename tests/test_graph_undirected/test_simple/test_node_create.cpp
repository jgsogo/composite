
#include <string>
#include "catch2/catch.hpp"
#include "graph_undirected.hpp"

namespace {

    struct NodeId {
        NodeId(std::string_view id, int idNum) : id(id), idNum(idNum) {}

        const std::string id;
        int idNum = 0;
    };

    struct EdgeId {
        EdgeId(std::string_view id, int idNum) : id(id), idNum(idNum) {}

        const std::string id;
        int idNum = 0;
    };

    using IdGraphUndirected = composite::GraphUndirected<NodeId, EdgeId>;
}

TEST_CASE("test_graph_undirected/test_simple | Node create", "[node_create]") {
    auto node = std::make_shared<IdGraphUndirected::GraphNode>("node", 23);
    REQUIRE(node->id == "node");
    REQUIRE(node->idNum == 23);
}

TEST_CASE("test_graph_undirected/test_simple | Edge create", "[edge_create]") {
    auto node1 = std::make_shared<IdGraphUndirected::GraphNode>("node1", 1);
    auto node2 = std::make_shared<IdGraphUndirected::GraphNode>("node2", 2);

    auto edge = node1->connect(node2, "edge", 11);
    REQUIRE(edge->id == "edge");
    REQUIRE(edge->idNum == 11);
}


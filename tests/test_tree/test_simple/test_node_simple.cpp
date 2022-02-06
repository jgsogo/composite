
#include <string>
#include <catch2/catch.hpp>
#include "tree.hpp"

namespace {

    struct NodeId {
        NodeId(std::string_view id, int idNum) : id(id), idNum(idNum) {}

        const std::string id;
        int idNum = 0;
    };

    using IdTree = composite::Tree<NodeId>;
}

TEST_CASE("test_tree/test_simple | Node create", "[trait_simple]") {
    auto node = std::make_shared<IdTree::TreeNode>("node", 23);
    REQUIRE(node->id == "node");
    REQUIRE(node->idNum == 23);
}

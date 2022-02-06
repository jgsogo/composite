
#include <string>
#include "catch2/catch.hpp"
#include "tree/tree.hpp"

namespace {

    struct NodeId {
        explicit NodeId(std::string_view id) : id(id) {}

        const std::string id;
        int depth = 0;
        std::string breadcrumb;
    };


    using IdNode = composite::Tree<NodeId>;

    // TALK: Argument-dependent-lookup (ADL) and why this function has to be written here: https://stackoverflow.com/questions/55878310/why-do-type-traits-not-work-with-types-in-namespace-scope
    inline void onNodeAdded(const std::vector<std::reference_wrapper<NodeId>> &gs, NodeId &p) {
        p.depth = gs.at(gs.size()-1).get().depth + 1;
        p.breadcrumb = "";
        for (auto g: gs) {
            p.breadcrumb += g.get().id + "::";
        }
        p.breadcrumb += p.id;
    }
}


TEST_CASE("test_tree/test_simple | onNodeAdded function", "[trait_addpart]") {
    REQUIRE(composite::_impl::tree::is_onNodeAdded<NodeId>());
}


TEST_CASE("test_tree/test_simple | onNodeAdded function is used", "[trait_addpart]") {
    auto root = std::make_shared<IdNode::TreeNode>("root");
    auto node1 = std::make_shared<IdNode::TreeNode>("node1");
    root->addChild(node1);

    auto node2 = std::make_shared<IdNode::TreeNode>("node2");
    auto node3 = std::make_shared<IdNode::TreeNode>("node3");
    node2->addChild(node3);
    root->addChild(node2);

    REQUIRE(root->id == "root");
    REQUIRE(root->depth == 0);
    REQUIRE(root->breadcrumb == "");

    REQUIRE(node1->id == "node1");
    REQUIRE(node1->depth == 1);
    REQUIRE(node1->breadcrumb == "root::node1");

    REQUIRE(node2->id == "node2");
    REQUIRE(node2->depth == 1);
    REQUIRE(node2->breadcrumb == "root::node2");

    REQUIRE(node3->id == "node3");
    REQUIRE(node3->depth == 2);
    REQUIRE(node3->breadcrumb == "root::node2::node3");
}

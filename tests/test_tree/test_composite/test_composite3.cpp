
#include <string>
#include "catch2/catch.hpp"
#include "tree_composite.hpp"

namespace {

    struct Data1 {
        Data1(std::string_view id) : data1(id) {}

        const std::string data1;
        std::string breadcrumb1;
    };

    struct Data2 {
        Data2(std::string_view id) : data2(id) {}

        const std::string data2;
        std::string breadcrumb2;
    };

    struct Data3 {
        Data3(std::string_view id) : data3(id) {}

        const std::string data3;
        std::string breadcrumb3;
    };

    using Trait1 = composite::Tree<Data1>;
    using Trait2 = composite::Tree<Data2>;
    using Trait3 = composite::Tree<Data3>;

    inline void onNodeAdded(const std::vector<std::reference_wrapper<Data1>> &gs, Data1 &p) {
        p.breadcrumb1 = "data1 - ";
        for (auto g: gs) {
            p.breadcrumb1 += g.get().data1 + "::";
        }
        p.breadcrumb1 += p.data1;
    }

    inline void onNodeAdded(const std::vector<std::reference_wrapper<Data3>> &gs, Data3 &p) {
        p.breadcrumb3 = "data3 - ";
        for (auto g: gs) {
            p.breadcrumb3 += g.get().data3 + "::";
        }
        p.breadcrumb3 += p.data3;
    }

    using CompositeTree = composite::TreeCompose<Trait1, Trait2, Trait3>::Tree;
}

TEST_CASE("test_tree/test_composite | TraitGroup create composite3", "[test_composite]") {
    auto group = std::make_shared<CompositeTree::TreeNode>("g1-1", "g1-2", "g1-3");
    REQUIRE(group->data1 == "g1-1");
    REQUIRE(group->data2 == "g1-2");
    REQUIRE(group->data3 == "g1-3");
}

TEST_CASE("test_tree/test_composite | Add part to composite3", "[test_composite]") {
    auto group = std::make_shared<CompositeTree::TreeNode>("g1-1", "g1-2", "g1-3");
    auto part = std::make_shared<CompositeTree::TreeNode>("p1-1", "p1-2", "p1-3");
    group->addChild(part);

    REQUIRE(group->data1 == "g1-1");
    REQUIRE(group->breadcrumb1 == "");
    REQUIRE(group->data2 == "g1-2");
    REQUIRE(group->breadcrumb2 == "");
    REQUIRE(group->data3 == "g1-3");
    REQUIRE(group->breadcrumb3 == "");

    REQUIRE(part->data1 == "p1-1");
    REQUIRE(part->breadcrumb1 == "data1 - g1-1::p1-1");
    REQUIRE(part->data2 == "p1-2");
    REQUIRE(part->breadcrumb2 == "");  // No addPart function for this one
    REQUIRE(part->data3 == "p1-3");
    REQUIRE(part->breadcrumb3 == "data3 - g1-3::p1-3");
}


TEST_CASE("test_tree/test_composite | Visitor composite3", "[test_composite]") {
    auto group1 = std::make_shared<CompositeTree::TreeNode>("g1-1", "g1-2", "g1-3");

    auto part1 = std::make_shared<CompositeTree::TreeNode>("p1-1", "p1-2", "p1-3");
    group1->addChild(part1);

    auto group2 = std::make_shared<CompositeTree::TreeNode>("g2-1", "g2-2", "g2-3");
    auto part2 = std::make_shared<CompositeTree::TreeNode>("p2-1", "p2-2", "p2-3");
    group2->addChild(part2);
    group1->addChild(group2);


    class Visitor : public CompositeTree::DFSVisitor {
    public:
        void visit(CompositeTree::NodeTypename &p) override {
            ids.push_back(p.data1);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    group1->accept(visitor);

    REQUIRE(visitor.ids == std::vector<std::string>{"p1-1", "p2-1", "g2-1", "g1-1"});
}

/*
TEST_CASE("test_composite | Visitor first trait", "[test_composite]") {
    auto group1 = std::make_shared<CompositeTrait::TraitGroup>("group1", 11);

    auto part1 = std::make_shared<CompositeTrait::TraitPart>("part1", 21);
    group1->addPart(part1);

    auto group2 = std::make_shared<CompositeTrait::TraitGroup>("group2", 12);
    auto part2 = std::make_shared<CompositeTrait::TraitPart>("part2", 22);
    group2->addPart(part2);
    group1->addPart(group2);


    class Visitor : public IDTrait::DFSVisitor {
    public:
        void visit(IDTrait::PartTypename &p) override {
            ids.push_back(p.idPart);
        }

        void visitGroup(IDTrait::GroupTypename &g) override {
            ids.push_back(g.idGroup);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;

    // TODO: Requires some work
    //group1->accept(visitor);

    //REQUIRE(visitor.ids == std::vector<std::string>{"part1", "part2", "group2", "group1"});
}

TEST_CASE("test_composite | Visitor second trait", "[test_composite]") {
    // TODO: Requires some work
}
*/
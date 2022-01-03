
#include <string>
#include "catch2/catch.hpp"
#include "../src/trait.hpp"


struct PartId {
    explicit PartId(std::string_view id) : idPart(id) {}

    const std::string idPart;
};

struct GroupId {
    explicit GroupId(std::string_view id) : idGroup(id) {}

    const std::string idGroup;
};

using IdTrait = composite::Trait<GroupId, PartId>;



TEST_CASE("Visitor DFS", "[trait_visitor]") {
    auto group1 = std::make_shared<IdTrait::TraitGroup>("group1");
    auto part1 = std::make_shared<IdTrait::TraitPart>("part1");
    group1->addPart(part1);

    auto group2 = std::make_shared<IdTrait::TraitGroup>("group2");
    auto part2 = std::make_shared<IdTrait::TraitPart>("part2");
    group2->addPart(part2);
    group1->addPart(group2);

    class Visitor : public IdTrait::DFSVisitor {
    public:
        virtual void visit(PartId &p) {
            ids.push_back(p.idPart);
        }

        virtual void visitGroup(GroupId &g) {
            ids.push_back(g.idGroup);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    group1->accept(visitor);

    REQUIRE(visitor.ids == std::vector<std::string>{"part1", "part2", "group2", "group1"});
}

TEST_CASE("Visitor BFS", "[trait_visitor]") {
    auto group1 = std::make_shared<IdTrait::TraitGroup>("group1");
    auto part1 = std::make_shared<IdTrait::TraitPart>("part1");
    group1->addPart(part1);

    auto group2 = std::make_shared<IdTrait::TraitGroup>("group2");
    auto part2 = std::make_shared<IdTrait::TraitPart>("part2");
    group2->addPart(part2);
    group1->addPart(group2);

    class Visitor : public IdTrait::BFSVisitor {
    public:
        virtual void visit(PartId &p) {
            ids.push_back(p.idPart);
        }

        virtual void visitGroup(GroupId &g) {
            ids.push_back(g.idGroup);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    group1->accept(visitor);

    REQUIRE(visitor.ids == std::vector<std::string>{"group1", "part1", "group2", "part2"});
}

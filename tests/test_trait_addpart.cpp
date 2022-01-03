
#include <string>
#include "catch2/catch.hpp"
#include "../src/trait.hpp"

namespace {

    struct PartId {
        explicit PartId(std::string_view id) : idPart(id) {}

        const std::string idPart;
        int depth = 0;
        std::string breadcrumb;
    };

    struct GroupId {
        explicit GroupId(std::string_view id) : idGroup(id) {}

        const std::string idGroup;
        int depth = 0;
        std::string breadcrumb;
    };

    using IdTrait = composite::Trait<GroupId, PartId>;

    // TALK: Argument-dependent-lookup (ADL) and why this function has to be written here: https://stackoverflow.com/questions/55878310/why-do-type-traits-not-work-with-types-in-namespace-scope
    inline void onPartAdded(const std::vector<std::reference_wrapper<GroupId>> &gs, PartId &p) {
        p.depth = gs.size();
        p.breadcrumb = "";
        for (auto g: gs) {
            p.breadcrumb += g.get().idGroup + "::";
        }
        p.breadcrumb += p.idPart;
    }
}

TEST_CASE("Test onPartAdded", "[trait_addpart]") {
    auto group1 = std::make_shared<IdTrait::TraitGroup>("group1");

    auto part1 = std::make_shared<IdTrait::TraitPart>("part1");
    group1->addPart(part1);

    auto group2 = std::make_shared<IdTrait::TraitGroup>("group2");
    auto part2 = std::make_shared<IdTrait::TraitPart>("part2");
    group2->addPart(part2);
    group1->addPart(group2);

    REQUIRE(group1->idGroup == "group1");
    REQUIRE(group1->depth == 0);
    REQUIRE(group1->breadcrumb == "");

    REQUIRE(part1->idPart == "part1");
    REQUIRE(part1->depth == 1);
    REQUIRE(part1->breadcrumb == "group1::part1");

    REQUIRE(group2->idGroup == "group2");
    REQUIRE(group2->depth == 0);
    REQUIRE(group2->breadcrumb == "");

    REQUIRE(part2->idPart == "part2");
    REQUIRE(part2->depth == 2);
    REQUIRE(part2->breadcrumb == "group1::group2::part2");
}

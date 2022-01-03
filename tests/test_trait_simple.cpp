
#include <string>
#include "catch2/catch.hpp"
#include "../src/trait.hpp"

namespace {

    struct PartId {
        PartId(std::string_view id, int idNum) : idPart(id), idNum(idNum) {}

        const std::string idPart;
        int idNum = 0;
    };

    struct GroupId {
        GroupId(std::string_view id, int idNum) : idGroup(id), idNum(idNum) {}

        const std::string idGroup;
        int idNum = 0;
    };

    using IdTrait = composite::Trait<GroupId, PartId>;
}

TEST_CASE("TraitGroup create", "[trait_simple]") {
    auto group = std::make_shared<IdTrait::TraitGroup>("group1", 23);
    REQUIRE(group->idGroup == "group1");
    REQUIRE(group->idNum == 23);
}

TEST_CASE("TraitPart create", "[trait_simple]") {
    auto part = std::make_shared<IdTrait::TraitPart>("part1", 42);
    REQUIRE(part->idPart == "part1");
    REQUIRE(part->idNum == 42);
}

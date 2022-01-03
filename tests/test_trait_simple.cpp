
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

TEST_CASE("TraitGroup create", "[trait_simple]") {
    auto group = std::make_shared<IdTrait::TraitGroup>("group1");
    REQUIRE(group->idGroup == "group1");
}

TEST_CASE("TraitPart create", "[trait_simple]") {
    auto part = std::make_shared<IdTrait::TraitPart>("part1");
    REQUIRE(part->idPart == "part1");
}

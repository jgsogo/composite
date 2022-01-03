
#include <string>
#include "catch2/catch.hpp"
#include "trait_compose.hpp"

namespace {

    struct PartId {
        PartId(std::string_view id) : idPart(id) {}

        const std::string idPart;
    };

    struct GroupId {
        GroupId(std::string_view id) : idGroup(id) {}

        const std::string idGroup;
    };

    using IdTrait = composite::Trait<GroupId, PartId>;

    struct PartIdNum {
        PartIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
    };

    struct GroupIdNum {
        GroupIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
    };

    using IdNumTrait = composite::Trait<GroupIdNum, PartIdNum>;

    using CompositeTrait = composite::TraitCompose<IdTrait, IdNumTrait>::Trait;
}

TEST_CASE("test_composite | TraitGroup create", "[test_composite]") {
    /*
    auto group = std::make_shared<CompositeTrait::TraitGroup>("group1", 23);
    REQUIRE(group->idGroup == "group1");
    REQUIRE(group->idNum == 23);
     */
}

TEST_CASE("test_composite | TraitPart create", "[test_composite]") {
    /*
    auto part = std::make_shared<CompositeTrait::TraitPart>("part1", 42);
    REQUIRE(part->idPart == "part1");
    REQUIRE(part->idNum == 42);
     */
}

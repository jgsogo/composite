
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


    struct PartIdNum {
        PartIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int depth = 0;
    };

    struct GroupIdNum {
        GroupIdNum(int idNum) : idNum(idNum) {}

        int idNum = 0;
        int depth = 0;
    };

    inline void onPartAdded(const std::vector<std::reference_wrapper<GroupIdNum>> &gs, PartIdNum &p) {
        p.depth = gs.size();
    }

    using IDTrait = composite::Trait<GroupId, PartId>;
    using IDNumTrait = composite::Trait<GroupIdNum, PartIdNum>;
    using CompositeTrait = composite::TraitCompose<IDTrait, IDNumTrait>::Trait;
}

TEST_CASE("test_composite | TraitGroup create", "[test_composite]") {
    auto group = std::make_shared<CompositeTrait::TraitGroup>("group1", 23);
    REQUIRE(group->idGroup == "group1");
    REQUIRE(group->idNum == 23);
    REQUIRE(group->depth == 0);
}

TEST_CASE("test_composite | TraitPart create", "[test_composite]") {
    auto part = std::make_shared<CompositeTrait::TraitPart>("part1", 42);
    REQUIRE(part->idPart == "part1");
    REQUIRE(part->idNum == 42);
    REQUIRE(part->depth == 0);
}

TEST_CASE("test_composite | Add part", "[test_composite]") {
    auto group = std::make_shared<CompositeTrait::TraitGroup>("group1", 23);
    auto part = std::make_shared<CompositeTrait::TraitPart>("part1", 42);
    group->addPart(part);

    REQUIRE(group->idGroup == "group1");
    REQUIRE(group->idNum == 23);
    REQUIRE(group->depth == 0);

    REQUIRE(part->idPart == "part1");
    REQUIRE(part->idNum == 42);
    REQUIRE(part->depth == 1);
}

TEST_CASE("test_composite | Visitor composite", "[test_composite]") {
    auto group1 = std::make_shared<CompositeTrait::TraitGroup>("group1", 11);

    auto part1 = std::make_shared<CompositeTrait::TraitPart>("part1", 21);
    group1->addPart(part1);

    auto group2 = std::make_shared<CompositeTrait::TraitGroup>("group2", 12);
    auto part2 = std::make_shared<CompositeTrait::TraitPart>("part2", 22);
    group2->addPart(part2);
    group1->addPart(group2);


    class Visitor : public CompositeTrait::DFSVisitor {
    public:
        void visit(CompositeTrait::PartTypename &p) override {
            ids.push_back(p.idPart);
        }

        void visitGroup(CompositeTrait::GroupTypename &g) override {
            ids.push_back(g.idGroup);
        }

    public:
        std::vector<std::string> ids;
    };
    Visitor visitor;
    group1->accept(visitor);

    REQUIRE(visitor.ids == std::vector<std::string>{"part1", "part2", "group2", "group1"});
}


TEST_CASE("test_composite | Visitor first trait", "[test_composite]") {
    auto group1 = std::make_shared<CompositeTrait::TraitGroup>("group1", 11);

    auto part1 = std::make_shared<CompositeTrait::TraitPart>("part1", 21);
    group1->addPart(part1);

    auto group2 = std::make_shared<CompositeTrait::TraitGroup>("group2", 12);
    auto part2 = std::make_shared<CompositeTrait::TraitPart>("part2", 22);
    group2->addPart(part2);
    group1->addPart(group2);

    {
        class Visitor : public IDTrait::DFSVisitor {
            /* A visitor dedicated to one of the traits */
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

        group1->accept(visitor);

        REQUIRE(visitor.ids == std::vector<std::string>{"part1", "part2", "group2", "group1"});
    }
    {
        class Visitor : public IDTrait::BFSVisitor {
            /* A visitor dedicated to one of the traits */
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

        group1->accept(visitor);

        REQUIRE(visitor.ids == std::vector<std::string>{"group1", "part1", "group2", "part2"});
    }
}

TEST_CASE("test_composite | Visitor second trait", "[test_composite]") {
    auto group1 = std::make_shared<CompositeTrait::TraitGroup>("group1", 11);

    auto part1 = std::make_shared<CompositeTrait::TraitPart>("part1", 21);
    group1->addPart(part1);

    auto group2 = std::make_shared<CompositeTrait::TraitGroup>("group2", 12);
    auto part2 = std::make_shared<CompositeTrait::TraitPart>("part2", 22);
    group2->addPart(part2);
    group1->addPart(group2);

    {
        class Visitor : public IDNumTrait::DFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDNumTrait::PartTypename &p) override {
                ids.push_back(p.idNum);
            }

            void visitGroup(IDNumTrait::GroupTypename &g) override {
                ids.push_back(g.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;

        group1->accept(visitor);

        REQUIRE(visitor.ids == std::vector<int>{21, 22, 12, 11});
    }
    {
        class Visitor : public IDNumTrait::BFSVisitor {
            /* A visitor dedicated to one of the traits */
        public:
            void visit(IDNumTrait::PartTypename &p) override {
                ids.push_back(p.idNum);
            }

            void visitGroup(IDNumTrait::GroupTypename &g) override {
                ids.push_back(g.idNum);
            }

        public:
            std::vector<int> ids;
        };
        Visitor visitor;

        group1->accept(visitor);

        REQUIRE(visitor.ids == std::vector<int>{11, 21, 12, 22});
    }
}

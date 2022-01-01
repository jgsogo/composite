#pragma once

#include "trait.hpp"

/*
 * Trait compose: combine the functionality of two traits to generate a new one
 * */


template<typename Trait1, typename Trait2>
struct TraitCompose {

    class PartTypename : public Trait1::PartTypename, public Trait2::PartTypename {
    public:
        template<typename ...Args1, typename ...Args2>
        explicit PartTypename(Args1... args1, Args2... args2) : Trait1::PartTypename(args2...), Trait2::PartTypename(args1...) {};

    };

    class GroupTypename : public Trait1::GroupTypename, public Trait2::GroupTypename {
    public:
        template<typename ...Args1, typename ...Args2>
        explicit GroupTypename(Args1... args1, Args2... args2) : Trait1::GroupTypename(args2...), Trait2::GroupTypename(args1...) {};

    };

    using TraitType = Trait<PartTypename, GroupTypename>;

    static void addedPartToGroup(typename TraitType::TraitGroup &group, typename TraitType::TraitPart &part) {
        // TODO: Trait1::addedPartToGroup(static_cast<typename Trait1::TraitGroup&>(group), static_cast<typename Trait1::TraitPart&>(part));
        // TODO: Trait2::addedPartToGroup(static_cast<typename Trait2::TraitGroup&>(group), static_cast<typename Trait2::TraitPart&>(part));
        Trait1::addedPartToGroup(static_cast<typename Trait1::GroupTypename &>(group), static_cast<typename Trait1::PartTypename &>(part));
        Trait2::addedPartToGroup(static_cast<typename Trait2::GroupTypename &>(group), static_cast<typename Trait2::PartTypename &>(part));
    }

    static bool addedGroupToGroup(typename TraitType::TraitGroup &group, typename TraitType::TraitGroup &part) {
        // TODO: auto t1 = Trait1::addedGroupToGroup(static_cast<typename Trait1::TraitGroup&>(group), static_cast<typename Trait1::TraitGroup&>(part));
        // TODO: auto t2 = Trait2::addedGroupToGroup(static_cast<typename Trait2::TraitGroup&>(group), static_cast<typename Trait2::TraitGroup&>(part));
        auto t1 = Trait1::addedGroupToGroup(static_cast<typename Trait1::GroupTypename &>(group), static_cast<typename Trait1::GroupTypename &>(part));
        auto t2 = Trait2::addedGroupToGroup(static_cast<typename Trait2::GroupTypename &>(group), static_cast<typename Trait2::GroupTypename &>(part));
        return t1 && t2; // TODO: what logical operator to use here? bitwise mask?
    }



    class ComposeInsertionVisitor;

    class TraitGroup : public TraitType::TraitGroup {
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : TraitType::TraitGroup(args...) {};

        virtual void addPart(std::shared_ptr<typename TraitType::TraitBase> item) {
            TraitType::TraitGroup::_addPart(item);
            //ComposeInsertionVisitor v{*this};
            //item->accept(v);
        }
    };

    /*
    class ComposeInsertionVisitor : public TraitType::InsertionVisitor {
    public:
        explicit ComposeInsertionVisitor(TraitGroup &group) : TraitType::InsertionVisitor(group) {}

        void visit(typename TraitType::TraitPart &part) override {
            addedPartToGroup(TraitType::InsertionVisitor::_group, part);
        }

        void visitGroup(typename TraitType::TraitGroup &group) override {
            addedGroupToGroup(TraitType::InsertionVisitor::_group, group);
        }
    };
    */
    struct Trait {
        using PartTypename = typename TraitType::PartTypename;
        using GroupTypename = typename TraitType::GroupTypename;
        using Visitor = typename TraitType::Visitor;
        using TraitBase = typename TraitType::TraitBase;
        using TraitPart = typename TraitType::TraitPart;
        using TraitGroup = TraitGroup;
        //using InsertionVisitor = typename TraitType::InsertionVisitor;
    };
};

#pragma once

#include <memory>
#include <vector>


/*
 * Anything we want to define as a trait, needs to inherit from these classes. A trait requires a virtual
 * base class, a visitor, groups and parts.
 * */



template<typename Part, typename Group>
struct Trait {
    using PartTypename = Part;
    using GroupTypename = Group;

    class TraitPart;

    class TraitGroup;

    static void addedPartToGroup(Group &group, Part &part) {

    }
    static void addedPartToGroup(TraitGroup &group, TraitPart &part) {
        addedPartToGroup(static_cast<Group&>(group), static_cast<Part&>(part));
    }

    static bool addedGroupToGroup(Group &group, Group &part) {
        return false;
    }
    static bool addedGroupToGroup(TraitGroup &group, TraitGroup &part) {
        return addedGroupToGroup(static_cast<Group&>(group), static_cast<Group&>(part));
    }

    class VisitorTrait {
    public:
        VisitorTrait() = default;

        virtual void visit(TraitPart &) = 0; //

        virtual bool enter(TraitGroup &) = 0; //{ return true; }

        virtual void exit(TraitGroup &) = 0; // {}
    };

    class Visitor : public VisitorTrait {
    public:
        Visitor() = default;

        virtual void visit(Part &) = 0; //

        virtual bool enter(Group &) = 0; //{ return true; }

        virtual void exit(Group &) = 0; // {}

    protected:
        void visit(TraitPart &p) final { this->visit(static_cast<Part &>(p)); }; //

        bool enter(TraitGroup &g) final { return this->enter(static_cast<Group &>(g)); }; //

        void exit(TraitGroup &g) final { this->exit(static_cast<Group &>(g)); }; //
    };

    class TraitBase {
    public:
        TraitBase() = default;

        virtual void accept(VisitorTrait &v) = 0;
    };

    class TraitPart : public TraitBase, public Part {
    public:
        template<typename ...Args>
        explicit TraitPart(Args... args) : Part(args...) {};

        void accept(VisitorTrait &v) override {
            v.visit(*this);
        };
    };

    class InsertionVisitor;

    class TraitGroup : public TraitBase, public Group {
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : Group(args...) {};

        virtual void addPart(std::shared_ptr<TraitBase> item) {
            _addPart(item);
            InsertionVisitor v{*this};
            item->accept(v);
        }

        void accept(VisitorTrait &v) override {
            if (v.enter(*this)) {
                for (auto &it: _children) {
                    it->accept(v);
                }
                v.exit(*this);
            }
        }

    protected:
        void _addPart(std::shared_ptr<TraitBase> item) {
            _children.push_back(item);
        }

    protected:
        std::vector<std::shared_ptr<TraitBase>> _children;
    };

    class InsertionVisitor : public VisitorTrait {
    public:
        explicit InsertionVisitor(TraitGroup &group) : _group(group) {}

        void visit(TraitPart &part) override {
            addedPartToGroup(_group, part);
        }

        bool enter(TraitGroup &group) override {
            return addedGroupToGroup(_group, group);
        }

        void exit(TraitGroup &) override {}

    protected:
        TraitGroup &_group;
    };
};

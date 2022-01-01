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
        addedPartToGroup(static_cast<Group &>(group), static_cast<Part &>(part));
    }

    static bool addedGroupToGroup(Group &group, Group &part) {
        return false;
    }

    static bool addedGroupToGroup(TraitGroup &group, TraitGroup &part) {
        return addedGroupToGroup(static_cast<Group &>(group), static_cast<Group &>(part));
    }

    template<typename ...Args>
    static void addedPart(/*TraitGroup &group, */Args &&... args) {
    }

    class VisitorTrait {
    public:
        VisitorTrait() = default;

        virtual void visit(TraitPart &) = 0; //

        virtual bool enterGroup(TraitGroup &) { return true; }; //

        virtual void visitGroup(TraitGroup &) = 0; //
        virtual void exitGroup(TraitGroup &) {}; //

        void _visitChildren(TraitGroup &group) {
            for (auto &it: group._children) {
                it->accept(*this);
            }
        }

        void _onGroup(TraitGroup &group) {
            if (this->enterGroup(group)) {
                // TODO: This is implementing DFS (pre-order) algorithm -- implement others
                this->_visitChildren(group);
                this->visitGroup(group);
                this->exitGroup(group);
            }
        }
    };

    class Visitor : public VisitorTrait {
    public:
        Visitor() = default;

        virtual void visit(Part &) = 0; //

        virtual void visitGroup(Group &) = 0; //
        virtual bool enterGroup(Group &) = 0; //
        virtual void exitGroup(Group &) = 0; //

    protected:
        void visit(TraitPart &p) final { this->visit(static_cast<Part &>(p)); }; //
        void visitGroup(TraitGroup &g) final { this->visitGroup(static_cast<Group &>(g)); }; //
        bool enterGroup(TraitGroup &g) final { return this->enterGroup(static_cast<Group &>(g)); }; //
        void exitGroup(TraitGroup &g) final { this->exitGroup(static_cast<Group &>(g)); }; //
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

    template<typename ...Args>
    class TPack;

    class TraitGroup : public TraitBase, public Group {
    public:
        friend class VisitorTrait;

        template<typename ...Args>
        explicit TraitGroup(Args... args) : Group(args...) {};

        virtual void addPart(std::shared_ptr<TraitBase> item) {
            _addPart(item);
            //InsertionVisitor v{*this};
            //item->accept(v);

            auto vPack = TPack<TraitGroup>{*this};
            item->accept(vPack);
        }

        void accept(VisitorTrait &v) override {
            v._onGroup(*this);
        }

    protected:
        void _addPart(std::shared_ptr<TraitBase> item) {
            _children.push_back(item);
        }

    protected:
        std::vector<std::shared_ptr<TraitBase>> _children;
    };

    template<typename ...Args>
    class TPack : public VisitorTrait {
    public:
        template<typename ...Args2>
        explicit TPack(Args2 &... args) : _chain(args...) {
            static_assert(sizeof...(args) <= 10, "You can't have more than 10 arguments!");
        }

        template<typename ...Args2, std::enable_if_t<(sizeof...(Args2) <= 10)> * = nullptr>
        static TPack<Args2...> make_from_tuple(std::tuple<Args2...> args) {
            return std::make_from_tuple<TPack<Args2...>>(args);
        }

        template<typename ...Args2, std::enable_if_t<(sizeof...(Args2) > 10)> * = nullptr>
        static TPack<Args2...> make_from_tuple(std::tuple<Args2...> args) {
            throw std::runtime_error("You can't have more that 10 nested groups");
        }

        void visit(TraitPart &part) override {
            auto tt = std::tuple_cat(_chain, std::tie(part));
            std::apply(&addedPart<Args&..., TraitPart&>, tt);
        }

        bool enterGroup(TraitGroup &group) override {
            auto tt = std::tuple_cat(_chain, std::tie(group));
            auto recurse = make_from_tuple(tt);
            recurse._visitChildren(group);
            return false;
        }

        void visitGroup(TraitGroup &group) override {
        }

    protected:
        std::tuple<Args &...> _chain;
    };

};

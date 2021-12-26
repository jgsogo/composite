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

    template<typename ...Args>
    static void addedPart(TraitGroup &group, Args&&... args) {

    }

    class VisitorTrait {
    public:
        VisitorTrait() = default;

        virtual void visit(TraitPart &) = 0; //

        virtual void visit(TraitGroup &) = 0; //

        virtual void _onGroup(TraitGroup& group) {
            // TODO: This is implementing DFS (pre-order) algorithm
            for (auto &it: group._children) {
                it->accept(*this);
            }
            this->visit(group);
        }
    };

    class Visitor : public VisitorTrait {
    public:
        Visitor() = default;

        virtual void visit(Part &) = 0; //

        virtual void visit(Group &) = 0; //
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
    template<typename ...Args>
    class TPack;

    class TraitGroup : public TraitBase, public Group {
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : Group(args...) {};

        virtual void addPart(std::shared_ptr<TraitBase> item) {
            _addPart(item);
            InsertionVisitor v{*this};
            item->accept(v);

            auto vPack = TPack<TraitGroup>{*this};
            item->accept(vPack);
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

    template<typename ...Args>
    class TPack : public VisitorTrait {
    public:
        template<typename ...Args2>
        explicit TPack(Args2&... args) : _chain(args...) {
            static_assert(sizeof...(args) <= 10, "You can't have more than 10 arguments!");
        }

        template<typename ...Args2, std::enable_if_t<(sizeof...(Args2) <= 10)>* = nullptr>
        static TPack<Args2...> make_from_tuple(std::tuple<Args2...> args) {
            return std::make_from_tuple<TPack<Args2...>>(args);
        }

        template<typename ...Args2, std::enable_if_t<(sizeof...(Args2) > 10)>* = nullptr>
        static TPack<Args2...> make_from_tuple(std::tuple<Args2...> args) {
            throw std::runtime_error("You can't have more that 10 nested groups");
        }

        void visit(TraitPart &part) override {
            //std::apply(&addedPart, std::tuple_cat(_chain, std::tie(part)));
        }

        bool enter(TraitGroup &group) override {
            auto tt = std::tuple_cat(_chain, std::tie(group));
            auto recurse = make_from_tuple(tt);
            group.accept(recurse);
            return false;
        }

        void exit(TraitGroup &) override {}

    protected:
        std::tuple<Args&...> _chain;
    };

};

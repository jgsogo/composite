#pragma once

#include <memory>
#include <vector>


/*
 * Anything we want to define as a trait, needs to inherit from these classes. A trait requires a virtual
 * base class, a visitor, groups and parts.
 * */


template <typename TGroup, typename TBase>
void addedItem2(TGroup& group, std::shared_ptr<TBase> item) {

}

template<typename Part, typename Group>
struct Trait;


template<typename Group, typename Part>
void addedItem(typename Trait<Part, Group>::GroupTypename& group, std::shared_ptr<typename Trait<Part, Group>::TraitBase> item) {

}


template<typename Part, typename Group>
struct Trait {
    using PartTypename = Part;
    using GroupTypename = Group;

    class Visitor {
    public:
        Visitor() = default;

        virtual void visit(Part &) = 0; //

        virtual bool enter(Group &) = 0; //{ return true; }

        virtual void exit(Group &) = 0; // {}
    };

    class TraitBase {
    public:
        TraitBase() = default;

        virtual void accept(Visitor &v) = 0;
    };

    class TraitPart : public TraitBase, public Part {
    public:
        template<typename ...Args>
        explicit TraitPart(Args... args) : Part(args...) {};

        void accept(Visitor &v) override {
            v.visit(*this);
        };
    };

    class TraitGroup : public TraitBase, public Group {
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : Group(args...) {};

        virtual void addPart(std::shared_ptr<TraitBase> item) {
            _children.push_back(item);
            addedItem<Group, Part>(*this, item);
        }

        void accept(Visitor &v) override {
            if (v.enter(*this)) {
                for (auto &it: _children) {
                    it->accept(v);
                }
                v.exit(*this);
            }
        }

    protected:
        std::vector<std::shared_ptr<TraitBase>> _children;
    };

};

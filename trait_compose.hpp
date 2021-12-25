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

    class TraitGroup : public TraitType::TraitGroup {
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : TraitType::TraitGroup(args...) {};

        virtual void addPart(std::shared_ptr<typename TraitType::TraitBase> item) {
            TraitType::TraitGroup::addPart(item);
            addedItem<typename Trait1::GroupTypename, typename Trait1::PartTypename>(*this, item);
            addedItem<typename Trait2::GroupTypename, typename Trait2::PartTypename>(*this, item);
        }
    };


    struct Trait {
        using PartTypename = typename TraitType::PartTypename;
        using GroupTypename = typename TraitType::GroupTypename;
        using Visitor = typename TraitType::Visitor;
        using TraitBase = typename TraitType::TraitBase;
        using TraitPart = typename TraitType::TraitPart;
        using TraitGroup = TraitGroup;
    };
    //using Trait = Trait<PartTypename, GroupTypename>;


    /*
    class Visitor : public Trait1::Visitor, public Trait2::Visitor {
    };

    class TraitPart : public Trait1::TraitPart, public Trait2::TraitPart {
    public:
        template<typename ...Args1, typename ...Args2>
        explicit TraitPart(Args1... args1, Args2... args2) : Trait1::TraitPart(args2...), Trait2::TraitPart(args2...) {};
    };

    class TraitGroup : public Trait1::TraitGroup, public Trait2::TraitGroup {
    };
     */
};

/*
template<typename Trait1, typename Trait2>
inline void addedItem(typename TraitCompose<Trait1, Trait2>::Trait::TraitGroup& group,
                      std::shared_ptr<typename TraitCompose<Trait1, Trait2>::Trait::TraitBase> item) {
    std::cout << ">>> messing with TraitCompose! " << std::endl;
}
 */

/*
template<typename Trait1, typename Trait2>
void TraitCompose<Trait1, Trait2>::Trait::addedItem(typename TraitCompose<Trait1, Trait2>::Trait::TraitGroup& group,
                                                    std::shared_ptr<typename TraitCompose<Trait1, Trait2>::Trait::TraitBase> item) {
    std::cout << "----";
}
 */

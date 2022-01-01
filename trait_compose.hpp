#pragma once

#include "trait.hpp"

/*
 * Trait compose: combine the functionality of two traits to generate a new one
 * */

template<typename Trait1, typename Trait2>
struct TraitCompose {
    using Trait1Type = Trait1;
    using Trait2Type = Trait2;

    class PartTypename : public Trait1Type::PartTypename, public Trait2Type::PartTypename {
    public:
        template<typename ...Args1, typename ...Args2>
        explicit PartTypename(Args1... args1, Args2... args2) : Trait1Type::PartTypename(args2...), Trait2Type::PartTypename(args1...) {};

    };

    class GroupTypename : public Trait1Type::GroupTypename, public Trait2Type::GroupTypename {
    public:
        template<typename ...Args1, typename ...Args2>
        explicit GroupTypename(Args1... args1, Args2... args2) : Trait1Type::GroupTypename(args2...), Trait2Type::GroupTypename(args1...) {};

    };

    using Trait = Trait<PartTypename, GroupTypename, TraitCompose<Trait1Type, Trait2Type>>;
};

template<typename Trait1, typename Trait2>
inline auto foo(std::vector<typename TraitCompose<Trait1, Trait2>::GroupTypename> &g, typename TraitCompose<Trait1, Trait2>::PartTypename &p) {
    if constexpr(Trait1::idAddFunction) {
        std::vector<typename Trait1::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
        foo(groups, (typename Trait1::PartTypename &) p);
    }
    if constexpr(Trait1::isCompose) {
        std::vector<typename Trait1::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
        foo<typename Trait1::CompositeTypename::Trait1Type, typename Trait1::CompositeTypename::Trait2Type>(groups,
                                                                                                            (typename Trait1::CompositeTypename::PartTypename &) p);
    }

    if constexpr(Trait2::idAddFunction) {
        std::vector<typename Trait2::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait2::GroupTypename &) item; });
        foo(groups, (typename Trait2::PartTypename &) p);
    }
    if constexpr(Trait2::isCompose) {
        std::vector<typename Trait2::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait2::GroupTypename &) item; });
        foo<typename Trait2::CompositeTypename::Trait1Type, typename Trait2::CompositeTypename::Trait2Type>(groups,
                                                                                                            (typename Trait2::CompositeTypename::PartTypename &) p);
    }
}



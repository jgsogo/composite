#pragma once

#include "trait.hpp"

/*
 * Trait compose: combine the functionality of two traits to generate a new one
 * */



template<typename Trait1, typename... Traits>
struct TraitCompose {
    using Trait1Type = Trait1;
    using Trait2Type = typename TraitCompose<Traits...>::Trait;
    using TraitsType = pack<Traits...>;

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

    using Trait = Trait<PartTypename, GroupTypename, TraitCompose<Trait1Type, Traits...>>;
};

template<typename Trait1>
struct TraitCompose<Trait1> {
    using Trait = Trait1;
};

template<typename Trait1, typename... Traits>
inline auto foo(std::vector<typename TraitCompose<Trait1, Traits...>::GroupTypename> &g, typename TraitCompose<Trait1, Traits...>::PartTypename &p) {
    if constexpr(Trait1::idAddFunction) {
        std::vector<typename Trait1::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
        foo(groups, (typename Trait1::PartTypename &) p);
    }
    static_assert(!Trait1::isCompose, "This is always false??? Remove next paragraph");
    if constexpr(Trait1::isCompose) {
        std::vector<typename Trait1::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
        foo<typename Trait1::CompositeTypename::Trait1Type, typename Trait1::CompositeTypename::Trait2Type>(groups,
                                                                                                            (typename Trait1::CompositeTypename::PartTypename &) p);
    }

    if constexpr(sizeof...(Traits) > 1) {
        std::vector<typename TraitCompose<Traits...>::Trait::GroupTypename> groups;
        std::transform(g.begin(), g.end(), std::back_inserter(groups),
                       [](auto &item) { return (typename TraitCompose<Traits...>::Trait::GroupTypename &) item; });
        foo<Traits...>(groups, (typename TraitCompose<Traits...>::Trait::PartTypename &) p);
    }

    if constexpr(sizeof...(Traits) == 1) {
        using TraitType = typename TraitCompose<Traits...>::Trait;
        if constexpr(TraitType::idAddFunction) {
            std::vector<typename TraitType::GroupTypename> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups),
                           [](auto &item) { return (typename TraitType::GroupTypename &) item; });
            foo(groups, (typename TraitType::PartTypename &) p);
        }
    }
}



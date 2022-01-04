#pragma once

#include "trait.hpp"
#include "impl/refl.hpp"

/*
 * Trait compose: combine the functionality of two traits to generate a new one
 * */

namespace composite {

    template<typename Trait1, typename... Traits>
    struct TraitCompose {
        using Trait1Type = Trait1;
        using Trait2Type = typename TraitCompose<Traits...>::Trait;
        using TraitsType = std::tuple<Traits...>;


        template<typename...Ts1>
        class PartTypenameT;

        template<typename ... Ts1, typename ... Ts2>
        class PartTypenameT<std::tuple<Ts1...>, std::tuple<Ts2...>> : public Trait1Type::PartTypename, public Trait2Type::PartTypename {
        public:
            explicit PartTypenameT(Ts1... args1, Ts2... args2) : Trait1Type::PartTypename(args1...), Trait2Type::PartTypename(args2...) {};
        };


        template<typename...Ts1>
        class GroupTypenameT;

        template<typename ... Ts1, typename ... Ts2>
        class GroupTypenameT<std::tuple<Ts1...>, std::tuple<Ts2...>> : public Trait1Type::GroupTypename, public Trait2Type::GroupTypename {
        public:
            explicit GroupTypenameT(Ts1... args1, Ts2... args2) : Trait1Type::GroupTypename(args1...), Trait2Type::GroupTypename(args2...) {};
        };

        // TALK: Ojo! conseguir los parámetros de un constructor (tiene limitaciones, como que encuentra sólo el primer ctor)
        using Trait1PartTypenameCtorParams = refl::as_tuple<typename Trait1Type::PartTypename>;
        using Trait2PartTypenameCtorParams = refl::as_tuple<typename Trait2Type::PartTypename>;
        using Trait1GroupTypenameCtorParams = refl::as_tuple<typename Trait1Type::GroupTypename>;
        using Trait2GroupTypenameCtorParams = refl::as_tuple<typename Trait2Type::GroupTypename>;
        static constexpr bool useComposite = Trait1Type::idAddFunction or TraitCompose<Traits...>::useComposite;

        using GroupTypename = GroupTypenameT<Trait1GroupTypenameCtorParams, Trait2GroupTypenameCtorParams>;
        using PartTypename = PartTypenameT<Trait1PartTypenameCtorParams, Trait2PartTypenameCtorParams>;
        using Trait = ::composite::Trait<GroupTypename, PartTypename,
                typename std::conditional<useComposite, TraitCompose<Trait1Type, Traits...>, void>::type>;
    };


    template<typename Trait1>
    struct TraitCompose<Trait1> {
        using Trait = Trait1;
        static constexpr bool useComposite = Trait1::idAddFunction;
    };

    template<typename Trait1, typename... Traits>
    inline auto onPartAdded(const std::vector<std::reference_wrapper<typename TraitCompose<Trait1, Traits...>::GroupTypename>> &g,
                            typename TraitCompose<Trait1, Traits...>::PartTypename &p) {
        if constexpr(Trait1::idAddFunction) {
            std::vector<std::reference_wrapper<typename Trait1::GroupTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return std::ref((typename Trait1::GroupTypename &) item); });
            onPartAdded(groups, (typename Trait1::PartTypename &) p);
        }

        static_assert(!Trait1::isCompose, "This is always false??? Remove next paragraph");
        /*
        if constexpr(Trait1::isCompose) {
            std::vector<std::reference_wrapper<typename Trait1::GroupTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups), [](auto &item) { return (typename Trait1::GroupTypename &) item; });
            onPartAdded<typename Trait1::CompositeTypename::Trait1Type, typename Trait1::CompositeTypename::Trait2Type>(groups,
                                                                                                                (typename Trait1::CompositeTypename::PartTypename &) p);
        }
         */

        if constexpr(sizeof...(Traits) > 1) {
            std::vector<std::reference_wrapper<typename TraitCompose<Traits...>::Trait::GroupTypename>> groups;
            std::transform(g.begin(), g.end(), std::back_inserter(groups),
                           [](auto &item) { return std::ref((typename TraitCompose<Traits...>::Trait::GroupTypename &) item); });
            onPartAdded<Traits...>(groups, (typename TraitCompose<Traits...>::Trait::PartTypename &) p);
        }

        if constexpr(sizeof...(Traits) == 1) {
            using TraitType = typename TraitCompose<Traits...>::Trait;
            if constexpr(TraitType::idAddFunction) {
                std::vector<std::reference_wrapper<typename TraitType::GroupTypename>> groups;
                std::transform(g.begin(), g.end(), std::back_inserter(groups),
                               [](auto &item) { return std::ref((typename TraitType::GroupTypename &) item); });
                onPartAdded(groups, (typename TraitType::PartTypename &) p);
            }
        }
    }

}

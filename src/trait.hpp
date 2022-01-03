#pragma once

#include <memory>
#include "impl/on_part_added.hpp"
#include "impl/visitor_trait.hpp"
#include "visitor.hpp"

/*
 * Anything we want to define as a trait, needs to inherit from these classes. A trait requires a virtual
 * base class, a visitor, groups and parts.
 * */

namespace composite {

    template<typename Group, typename Part, typename CompositeType = void>
    struct Trait {
        using PartTypename = Part;
        using GroupTypename = Group;
        using CompositeTypename = CompositeType;


        static constexpr bool isCompose = not std::is_same_v<CompositeTypename, void>;
        static constexpr bool idAddFunction = _impl::is_add_function<GroupTypename, PartTypename>();

        using Visitor = Visitor<Trait<GroupTypename, Part, CompositeTypename>>;


        class TraitPart;

        class TraitGroup;

        class TraitBase {
        public:
            TraitBase() = default;

            virtual void accept(_impl::VisitorTrait<TraitGroup, TraitPart> &v) = 0;
        };

        class TraitPart : public TraitBase, public PartTypename {
        public:
            using PartTypename = PartTypename;
        public:
            template<typename ...Args>
            explicit TraitPart(Args... args) : PartTypename(args...) {};

            void accept(_impl::VisitorTrait<TraitGroup, TraitPart> &v) override {
                v.visit(*this);
            };
        };

        class TraitGroup : public TraitBase, public GroupTypename {
        public:
            using GroupTypename = GroupTypename;
        public:
            friend class _impl::VisitorTrait<TraitGroup, TraitPart>;

            template<typename ...Args>
            explicit TraitGroup(Args... args) : GroupTypename(args...) {};

            virtual void addPart(std::shared_ptr<TraitBase> item) {
                __addPart(item);
            }

            void accept(_impl::VisitorTrait<TraitGroup, TraitPart> &v) override {
                v._onGroup(*this);
            }

        protected:
            template<bool enable = idAddFunction>
            typename std::enable_if<enable || isCompose>::type
            __addPart(std::shared_ptr<TraitBase> item) {
                _children.push_back(item);
                auto vPack = AddPartVisitor{*this};
                item->accept(vPack);
            }

            template<bool enable = idAddFunction>
            typename std::enable_if<!enable && !isCompose>::type
            __addPart(std::shared_ptr<TraitBase> item) {
                _children.push_back(item);
            }

        protected:
            std::vector<std::shared_ptr<TraitBase>> _children;
        };

        /*
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
                //std::apply(&addedPart<TraitGroup, Args &..., TraitPart &>, tt);
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
         */


        class AddPartVisitor : public _impl::VisitorTrait<TraitGroup, TraitPart> {
        public:
            explicit AddPartVisitor(TraitGroup &gr) {
                _chain.template emplace_back(gr);
            }

            AddPartVisitor(std::vector<std::reference_wrapper<GroupTypename>> &v, TraitGroup &gr) : _chain(v) {
                _chain.template emplace_back(gr);
            }

            template<typename...T>
            struct OnPartAddedCaller;

            template<typename T1, typename... T2>
            struct OnPartAddedCaller<T1, std::tuple<T2...>> {
                static auto call(const std::vector<std::reference_wrapper<GroupTypename>> &g, PartTypename &p) {
                    onPartAdded<T1, T2...>(g, p);
                }
            };

            void visit(TraitPart &part) override {
                if constexpr(isCompose) {
                    OnPartAddedCaller<typename CompositeTypename::Trait1Type, typename CompositeTypename::TraitsType>::call(_chain, (PartTypename &) part);
                } else {
                    onPartAdded(_chain, (PartTypename &) part);
                }
            }

            bool enterGroup(TraitGroup &group) override {
                auto recurse = AddPartVisitor{_chain, group};
                recurse._visitChildren(group);
                return false;
            }

            void visitGroup(TraitGroup &group) override {
            }

        protected:
            std::vector<std::reference_wrapper<GroupTypename>> _chain;
        };
    };
}
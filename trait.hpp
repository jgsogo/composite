#pragma once

#include <memory>
#include <vector>


/*
 * Anything we want to define as a trait, needs to inherit from these classes. A trait requires a virtual
 * base class, a visitor, groups and parts.
 * */

template<typename... Args>
struct pack {
};

struct S {
};

void foo(const std::vector<std::reference_wrapper<S>> &, S &);

template<class...> using void_t = void;

template<class, class, class = void>
struct is_add_function : std::false_type {
};

template<class T1, class T2>
struct is_add_function<T1, T2, void_t<decltype(foo(std::declval<const std::vector<std::reference_wrapper<T1>> &>(), std::declval<T2 &>()))>> : std::true_type {
};


template<typename Group, typename Part, typename CompositeType = void>
struct Trait {
    using PartTypename = Part;
    using GroupTypename = Group;
    using CompositeTypename = CompositeType;

    class TraitPart;

    class TraitGroup;

    static constexpr bool isCompose = not std::is_same_v<CompositeTypename, void>;
    static constexpr bool idAddFunction = is_add_function<GroupTypename, Part>();

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

    class TraitGroup : public TraitBase, public Group {
    public:
        friend class VisitorTrait;

        template<typename ...Args>
        explicit TraitGroup(Args... args) : Group(args...) {};

        virtual void addPart(std::shared_ptr<TraitBase> item) {
            __addPart(item);
        }

        void accept(VisitorTrait &v) override {
            v._onGroup(*this);
        }

    protected:
        template<bool enable = idAddFunction>
        typename std::enable_if<enable || isCompose>::type
        __addPart(std::shared_ptr<TraitBase> item) {
            _children.push_back(item);
            auto vPack = TPack2{*this};
            item->accept(vPack);
        }

        template<bool enable = idAddFunction>
        typename std::enable_if<!enable && !isCompose>::type
        __addPart(std::shared_ptr<TraitBase> item) {
            _children.push_back(item);
        }

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


    class TPack2 : public VisitorTrait {
    public:
        explicit TPack2(TraitGroup &gr) {
            _chain.template emplace_back(gr);
        }

        TPack2(std::vector<std::reference_wrapper<GroupTypename>> &v, TraitGroup &gr) : _chain(v) {
            _chain.template emplace_back(gr);
        }

        template<typename...T>
        struct FooCaller;

        template<typename T1, typename... T2>
        struct FooCaller<T1, pack<T2...>> {
            static auto call(const std::vector<std::reference_wrapper<GroupTypename>> &g, PartTypename &p) {
                foo < T1, T2... > (g, p);
            }
        };

        void visit(TraitPart &part) override {
            if constexpr(isCompose) {
                FooCaller<typename CompositeTypename::Trait1Type, typename CompositeTypename::TraitsType>::call(_chain, (PartTypename &) part);
            } else {
                foo(_chain, (PartTypename &) part);
            }
        }

        bool enterGroup(TraitGroup &group) override {
            auto recurse = TPack2{_chain, group};
            recurse._visitChildren(group);
            return false;
        }

        void visitGroup(TraitGroup &group) override {
        }

    protected:
        std::vector<std::reference_wrapper<GroupTypename>> _chain;
    };
};

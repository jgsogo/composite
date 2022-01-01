#pragma once

#include <memory>
#include <vector>
#include <iostream>


/*
 * Anything we want to define as a trait, needs to inherit from these classes. A trait requires a virtual
 * base class, a visitor, groups and parts.
 * */


template<typename T, typename ...Args>
static typename std::enable_if<std::is_base_of<void, T>::value, void>::type
addedPart(Args &&... args) {
    std::cout << "trait.hpp -- addedPart" << std::endl;
}


//template<typename Part, typename Group>
//void foo(std::vector<Group>& , Part&);
struct S {
};

void foo(std::vector<S> &, S &);

template<class...> using void_t = void;

template<class, class, class = void>
struct is_add_function : std::false_type {
};

template<class T1, class T2>
struct is_add_function<T1, T2, void_t<decltype(foo(std::declval<std::vector<T1> &>(), std::declval<T2 &>()))>> : std::true_type {
};



template<typename Part, typename Group, typename CompositeType = void>
struct Trait {
    using PartTypename = Part;
    using GroupTypename = Group;

    class TraitPart;

    class TraitGroup;

    static constexpr bool is_add_function_v = is_add_function<Group, Part>();
    static constexpr bool isCompose = not std::is_same_v<CompositeType, void>;

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


    //template<typename ...Args>
    //static void addedPart(/*TraitGroup &group, */Args &&... args) {
    //    std::cout << "trait.hpp -- addPart" << std::endl;
    //}


    template<typename T, typename ...Args>
    static typename std::enable_if<std::is_base_of<Group, T>::value, void>::type
    addedPart(Args &&... args);/* {
        std::cout << "trait.hpp -- addedPart" << std::endl;
    }*/

    //static void foo(std::vector<Group>& , Part&);
    //{
    //}

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
            __addPart(item);
            //_addPart(item);

            //auto vPack = TPack<TraitGroup>{*this};
            //item->accept(vPack);
        }

        void accept(VisitorTrait &v) override {
            v._onGroup(*this);
        }

    protected:
        template<bool enable = is_add_function_v>
        typename std::enable_if<enable || isCompose>::type
        __addPart(std::shared_ptr<TraitBase> item) {
            std::cout << "Impl1 -- hay funcion" << std::endl;
            _children.push_back(item);
            auto vPack = TPack2{*this};
            item->accept(vPack);
        }

        template<bool enable = is_add_function_v>
        typename std::enable_if<!enable && !isCompose>::type
        __addPart(std::shared_ptr<TraitBase> item) {
            std::cout << "Impl2  -- no hay" << std::endl;
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

        TPack2(std::vector<GroupTypename> &v, TraitGroup &gr) : _chain(v) {
            _chain.template emplace_back(gr);
        }

        void visit(TraitPart &part) override {
            //__visit(part);
            if constexpr(isCompose) {
                fooCompose<typename CompositeType::Trait1Type, typename CompositeType::Trait2Type>(_chain, (PartTypename &) part);
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
        std::vector<GroupTypename> _chain;
    };


};

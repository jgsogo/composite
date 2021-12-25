#pragma once

#include <string>
#include "trait.hpp"


/*
 * This logs the depth of every item in the graph
 * */

class ItemDepth {
public:
    explicit ItemDepth() = default;

public:
    int depth = 0;
};

struct DepthTrait : public Trait<ItemDepth, ItemDepth> {
    class TraitGroup : public Trait<ItemDepth, ItemDepth>::TraitGroup {
    public:
        //using Trait<ItemDepth, ItemDepth>::TraitGroup::addPart;
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : Trait<ItemDepth, ItemDepth>::TraitGroup(args...) {};

        void addPart(std::shared_ptr<TraitBase> item) override;
        /*
        void addPart(std::shared_ptr<TraitPart> item) {
            item->depth = this->depth + 1;
            _children.push_back(item);
        }

        void addPart(std::shared_ptr<TraitGroup> item) {
            for (auto &it: item->_children) {
                //it->depth = 23;
            }
            std::cout << "****" << std::endl;
            _children.push_back(item);
        }
         */

    };
};



//using DepthTrait = Trait<ItemDepth, ItemDepth>;

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


/*
struct DepthTrait : public Trait<ItemDepth, ItemDepth> {
    class TraitGroup : public Trait<ItemDepth, ItemDepth>::TraitGroup {
    public:
        //using Trait<ItemDepth, ItemDepth>::TraitGroup::addPart;
    public:
        template<typename ...Args>
        explicit TraitGroup(Args... args) : Trait<ItemDepth, ItemDepth>::TraitGroup(args...) {};

        void addPart(std::shared_ptr<TraitBase> item) override;

    };
};
*/


using DepthTrait = Trait<ItemDepth, ItemDepth>;

template <>
void DepthTrait::addedItem(DepthTrait::TraitGroup& group, std::shared_ptr<DepthTrait::TraitBase> item);



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


using DepthTrait = Trait<ItemDepth, ItemDepth>;

template<>
void DepthTrait::addedPartToGroup(ItemDepth& group, ItemDepth& part);

template<>
bool DepthTrait::addedGroupToGroup(DepthTrait::TraitGroup& group, DepthTrait::TraitGroup& part);

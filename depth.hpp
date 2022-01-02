#pragma once

#include <string>
#include <iostream>
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

void onPartAdded(const std::vector<std::reference_wrapper<ItemDepth>> &, ItemDepth &);

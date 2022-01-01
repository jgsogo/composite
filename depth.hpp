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

inline auto foo(std::vector<ItemDepth>&, ItemDepth&) {
    std::cout << "ItemDepth --- la caña de españa" << std::endl;
}

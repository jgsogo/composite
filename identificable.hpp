#pragma once

#include <string>
#include "trait.hpp"


/*
 * Try to add something just to parts
 * */

class PartId {
public:
    explicit PartId(std::string_view id) : id(id) {}

public:
    std::string id;
};

/*
class GroupId {
public:
    explicit GroupId(std::string_view id) : id(id) {}

public:
    std::string id;
};
*/

using IdenticableTrait = Trait<PartId, PartId>;

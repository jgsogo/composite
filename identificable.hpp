#pragma once

#include <string>
#include "src/trait.hpp"


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

using IdenticableTrait = composite::Trait<PartId, PartId>;

// TALK: Argument-dependent-lookup (ADL) and why this function has to be written here: https://stackoverflow.com/questions/55878310/why-do-type-traits-not-work-with-types-in-namespace-scope
inline void onPartAdded(const std::vector<std::reference_wrapper<PartId>> &, PartId &) {
    std::cout << "PartId --- la caña de españa" << std::endl;
}

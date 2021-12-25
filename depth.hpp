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

/*
template <typename TGroup, typename TBase>
void addedItem3<TGroup, TBase, std::enable_if_t<std::is_base_of_v<ItemDepth, TGroup>>>(TGroup& group, std::shared_ptr<TBase> item) {
    std::cout << "*******" << std::endl;
    //InsertionVisitor insertionVisitor{group.depth + 1};
    //item->accept(insertionVisitor);
}
 */

/*
template <typename TGroup, typename TBase>
inline typename std::enable_if<std::is_base_of<TGroup, ItemDepth>::value, void>::type
addedItem(TGroup& group, std::shared_ptr<TBase> item) {
    std::cout << ">>> messing with ItemDepth! " << std::endl;
}
*/

template <>
inline void addedItem<ItemDepth, ItemDepth>(Trait<ItemDepth, ItemDepth>::GroupTypename& group, std::shared_ptr<Trait<ItemDepth, ItemDepth>::TraitBase> item);




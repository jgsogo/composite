#include "depth.hpp"

namespace {

    class InsertionDepthVisitor : public DepthTrait::Visitor {
    public:
        explicit InsertionDepthVisitor(int baseDepth) : baseDepth(baseDepth) {};

        void visit(ItemDepth &item) override {
            item.depth += baseDepth;
        }

        bool enter(ItemDepth &item) override {
            item.depth += baseDepth;
            return true;
        }

        void exit(ItemDepth &item) override {
        }

    protected:
        const int baseDepth;
    };

}

template<>
void Trait<ItemDepth, ItemDepth>::addedPartToGroup(ItemDepth& group, ItemDepth& part) {
    part.depth = group.depth + 1;
}

template<>
bool DepthTrait::addedGroupToGroup(DepthTrait::TraitGroup& group, DepthTrait::TraitGroup& part) {
    int groupDepth = group.depth + 1;
    InsertionDepthVisitor insertionVisitor{groupDepth};
    part.accept(insertionVisitor);
    return false;
}

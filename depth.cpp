#include "depth.hpp"

namespace {

    class InsertionDepthVisitor : public DepthTrait::Visitor {
    public:
        explicit InsertionDepthVisitor(int baseDepth) : depth(baseDepth) {};

        void visit(ItemDepth &item) override {
            item.depth = depth;
        }

        void visitGroup(ItemDepth &item) override {
            item.depth = depth;
        }

        bool enterGroup(ItemDepth &) override {
            depth++;
            return true;
        }

        void exitGroup(ItemDepth &) override {
            depth--;
        }


    protected:
        int depth;
    };

}

void onPartAdded(const std::vector<std::reference_wrapper<ItemDepth>> &, ItemDepth &) {
    std::cout << "ItemDepth --- la caña de españa" << std::endl;
}

/*
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
*/

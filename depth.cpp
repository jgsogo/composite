#include "depth.hpp"

namespace {
    class InsertionVisitor : public DepthTrait::Visitor {
    public:
        explicit InsertionVisitor(int baseDepth) : baseDepth(baseDepth) {};

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


void DepthTrait::TraitGroup::addPart(std::shared_ptr<TraitBase> item) {
    InsertionVisitor insertionVisitor{this->depth + 1};
    item->accept(insertionVisitor);

    Trait<ItemDepth, ItemDepth>::TraitGroup::addPart(item);
}

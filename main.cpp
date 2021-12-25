#include <iostream>

#include "identificable.hpp"
#include "depth.hpp"
#include "trait_compose.hpp"

void depth() {
    /* DEPTH */
    class DepthVisitor : public DepthTrait::Visitor {
    public:
        void visit(ItemDepth &item) override {
            std::cout << "DepthVisitor::visit " << item.depth << std::endl;
        }

        bool enter(ItemDepth &item) override {
            std::cout << "DepthVisitor::enter " << item.depth << std::endl;
            return true;
        }

        void exit(ItemDepth &item) override {
            std::cout << "DepthVisitor::exit " << item.depth << std::endl;
        }

    };
    DepthVisitor depthVisitor;

    // Create a part
    auto p1 = std::make_shared<DepthTrait::TraitPart>();
    auto p2 = std::make_shared<DepthTrait::TraitPart>();
    auto p3 = std::make_shared<DepthTrait::TraitPart>();

    auto g1 = std::make_shared<DepthTrait::TraitGroup>();
    auto g2 = std::make_shared<DepthTrait::TraitGroup>();
    g1->addPart(p1);
    g1->addPart(p2);
    g1->addPart(g2);
    g2->addPart(p3);

    g1->accept(depthVisitor);
}

void identificable() {
    /* IDENTIFICABLE */
    class IdVisitor : public IdenticableTrait::Visitor {
    public:
        void visit(PartId &item) override {
            std::cout << std::string(tabs, ' ') << "part: " << item.id << std::endl;
        }

        bool enter(PartId &item) override {
            std::cout << std::string(tabs, ' ') << item.id << ":" << std::endl;
            tabs++;
            return true;
        }

        void exit(PartId &item) override {
            std::cout << std::string(tabs, ' ') << "<" << std::endl;
            tabs--;
        }

    public:
        int tabs = 0;
    };
    IdVisitor idVisitor;
    // Create a part
    auto p1 = std::make_shared<IdenticableTrait::TraitPart>("part1");
    auto p2 = std::make_shared<IdenticableTrait::TraitPart>("part2");
    auto p3 = std::make_shared<IdenticableTrait::TraitPart>("part3");

    auto g1 = std::make_shared<IdenticableTrait::TraitGroup>("group1");
    auto g2 = std::make_shared<IdenticableTrait::TraitGroup>("group2");
    g1->addPart(p1);
    g1->addPart(p2);
    g1->addPart(g2);
    g2->addPart(p3);

    g1->accept(idVisitor);
}

int main() {
    std::cout << std::endl << std::endl << "IDENTIFICABLE" << std::endl;
    identificable();

    std::cout << std::endl << std::endl << "DEPTH" << std::endl;
    depth();

    std::cout << std::endl << std::endl << "IDDEPTH" << std::endl;
    // Composition
    using IDDepth = TraitCompose<IdenticableTrait, DepthTrait>::Trait;

    auto p1 = std::make_shared<IDDepth::TraitPart>("part1");
    auto p2 = std::make_shared<IDDepth::TraitPart>("part2");
    auto p3 = std::make_shared<IDDepth::TraitPart>("part3");

    auto g1 = std::make_shared<IDDepth::TraitGroup>("group1");
    auto g2 = std::make_shared<IDDepth::TraitGroup>("group2");

    g1->addPart(p1);
    g1->addPart(p2);
    g1->addPart(g2);
    g2->addPart(p3);

    class IDDepthVisitor : public IDDepth::Visitor {
    public:
        void visit(IDDepth::PartTypename &item) override {
            std::cout << std::string(tabs, ' ') << "part: " << item.id << " (" << item.depth << ")" << std::endl;
        }

        bool enter(IDDepth::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << item.id << " (" << item.depth << ")" << ":" << std::endl;
            tabs++;
            return true;
        }

        void exit(IDDepth::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << "<" << std::endl;
            tabs--;
        }

    public:
        int tabs = 0;
    };
    IDDepthVisitor v;

    g1->accept(v);

    return 0;
}

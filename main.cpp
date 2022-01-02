#include <iostream>

#include "identificable.hpp"
#include "depth.hpp"
#include "autonum.hpp"
#include "trait_compose.hpp"

void depth() {
    /* DEPTH */
    class DepthVisitor : public DepthTrait::Visitor {
    public:
        void visit(ItemDepth &item) override {
            std::cout << "DepthVisitor::visit " << item.depth << std::endl;
        }

        void visitGroup(ItemDepth &item) override {
            std::cout << "DepthVisitor::visitGroup " << item.depth << std::endl;
        }

        bool enterGroup(ItemDepth &item) override {
            std::cout << "DepthVisitor::enterGroup " << item.depth << std::endl;
            return true;
        }

        void exitGroup(ItemDepth &item) override {
            std::cout << "DepthVisitor::exitGroup " << item.depth << std::endl;
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

        void visitGroup(PartId &item) override {
            //std::cout << std::string(tabs, ' ') << item.id << ":" << std::endl;
        }

        bool enterGroup(PartId &item) override {
            std::cout << std::string(tabs, ' ') << item.id << ":" << std::endl;
            tabs++;
            return true;
        }

        void exitGroup(PartId &item) override {
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

void idDepth() {
    // Composition
    using IDDepth = TraitCompose<IdenticableTrait, DepthTrait>::Trait;

    auto p1 = std::make_shared<IDDepth::TraitPart>("part1");
    auto p2 = std::make_shared<IDDepth::TraitPart>("part2");
    auto p3 = std::make_shared<IDDepth::TraitPart>("part3");

    auto g1 = std::make_shared<IDDepth::TraitGroup>("group1");
    auto g2 = std::make_shared<IDDepth::TraitGroup>("group2");

    g1->addPart(p1);
    g1->addPart(p2);
    g2->addPart(p3);
    g1->addPart(g2);

    class IDDepthVisitor : public IDDepth::Visitor {
    public:
        void visit(IDDepth::PartTypename &item) override {
            std::cout << std::string(tabs, ' ') << "part: " << item.id << " (" << item.depth << ")" << std::endl;
        }

        bool enterGroup(IDDepth::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << item.id << ":" << std::endl;
            tabs++;
            return true;
        }

        void visitGroup(IDDepth::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << item.id << " (" << item.depth << ")" << ":" << std::endl;
        }

        void exitGroup(IDDepth::GroupTypename &item) override {
            //std::cout << std::string(tabs, ' ') << "<" << std::endl;
            tabs--;
        }

    public:
        int tabs = 0;
    };
    IDDepthVisitor v;

    g1->accept(v);
}


void traits3() {
    // Composition
    //using IDDepth = TraitCompose<IdenticableTrait, DepthTrait>::Trait;
    using IDDepthAutoNum = TraitCompose<IdenticableTrait, DepthTrait, AutoNumTrait>::Trait;

    auto p1 = std::make_shared<IDDepthAutoNum::TraitPart>("part1");
    auto p2 = std::make_shared<IDDepthAutoNum::TraitPart>("part2");
    auto p3 = std::make_shared<IDDepthAutoNum::TraitPart>("part3");

    auto g1 = std::make_shared<IDDepthAutoNum::TraitGroup>("group1");
    auto g2 = std::make_shared<IDDepthAutoNum::TraitGroup>("group2");


    g1->addPart(p1);
    g1->addPart(p2);
    g2->addPart(p3);
    g1->addPart(g2);

    class IDDepthAutoNumVisitor : public IDDepthAutoNum::Visitor {
    public:
        void visit(IDDepthAutoNum::PartTypename &item) override {
            std::cout << std::string(tabs, ' ') << "part: " << item.id << " (" << item.depth << ")" << std::endl;
        }

        bool enterGroup(IDDepthAutoNum::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << item.id << ":" << std::endl;
            tabs++;
            return true;
        }

        void visitGroup(IDDepthAutoNum::GroupTypename &item) override {
            std::cout << std::string(tabs, ' ') << item.id << " (" << item.depth << ")" << ":" << std::endl;
        }

        void exitGroup(IDDepthAutoNum::GroupTypename &item) override {
            //std::cout << std::string(tabs, ' ') << "<" << std::endl;
            tabs--;
        }

    public:
        int tabs = 0;
    };
    IDDepthAutoNumVisitor v;

    g1->accept(v);
}

int main() {
    std::cout << std::endl << std::endl << "IDENTIFICABLE" << std::endl;
    identificable();

    std::cout << std::endl << std::endl << "DEPTH" << std::endl;
    depth();

    std::cout << std::endl << std::endl << "IDDEPTH" << std::endl;
    idDepth();

    std::cout << std::endl << std::endl << "ID+DEPTH+AUTONUM" << std::endl;
    traits3();

    return 0;
}

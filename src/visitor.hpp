#pragma once

namespace composite {

    template<typename VisitorTrait>
    class Visitor : public VisitorTrait {
        using TraitGroup = typename VisitorTrait::TraitGroup;
        using TraitPart = typename VisitorTrait::TraitPart;
    public:
        Visitor() = default;

        virtual void visit(typename TraitPart::PartTypename &) {}

        virtual bool enterGroup(typename TraitGroup::GroupTypename &) { return true; }

        virtual void visitGroup(typename TraitGroup::GroupTypename &) {}

        virtual void exitGroup(typename TraitGroup::GroupTypename &) {}

    protected:
        void visit(TraitPart &p) final { this->visit(static_cast<typename TraitPart::PartTypename &>(p)); }

        void visitGroup(TraitGroup &g) final { this->visitGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }

        bool enterGroup(TraitGroup &g) final { return this->enterGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }

        void exitGroup(TraitGroup &g) final { this->exitGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }
    };

}

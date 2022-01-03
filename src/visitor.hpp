#pragma once

namespace composite {

    template<typename Trait>
    class Visitor : public _impl::VisitorTrait<typename Trait::TraitGroup, typename Trait::TraitPart> {
        using TraitGroup = typename Trait::TraitGroup;
        using TraitPart = typename Trait::TraitPart;
    public:
        Visitor() = default;

        virtual void visit(typename TraitPart::PartTypename &) = 0; //

        virtual void visitGroup(typename TraitGroup::GroupTypename &) = 0; //
        virtual bool enterGroup(typename TraitGroup::GroupTypename &) = 0; //
        virtual void exitGroup(typename TraitGroup::GroupTypename &) = 0; //

    protected:
        void visit(TraitPart &p) final { this->visit(static_cast<typename TraitPart::PartTypename &>(p)); }; //
        void visitGroup(TraitGroup &g) final { this->visitGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }; //
        bool enterGroup(TraitGroup &g) final { return this->enterGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }; //
        void exitGroup(TraitGroup &g) final { this->exitGroup(static_cast<typename TraitGroup::GroupTypename &>(g)); }; //
    };

}

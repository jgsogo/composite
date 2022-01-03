#pragma once

#include "visitor_trait.hpp"
#include "../visitor.hpp"

namespace composite::_impl {

    template<typename TGroup, typename TPart, typename ChildTGroup, typename ChildTPart, template<typename, typename> typename ImplVisitor>
    class VisitorWrapperCast : public ImplVisitor<ChildTGroup, ChildTPart> {
    public:
        explicit VisitorWrapperCast(Visitor<ImplVisitor<TGroup, TPart>> &visitor) : visitor(visitor) {}

        void visit(typename ImplVisitor<ChildTGroup, ChildTPart>::TraitPart &p) final {
            visitor.visit(static_cast<typename TPart::PartTypename &>(p));
        }

        void visitGroup(typename ImplVisitor<ChildTGroup, ChildTPart>::TraitGroup &g) final {
            visitor.visitGroup(static_cast<typename TGroup::GroupTypename &>(g));
        }

        bool enterGroup(typename ImplVisitor<ChildTGroup, ChildTPart>::TraitGroup &g) final {
            return visitor.enterGroup(static_cast<typename TGroup::GroupTypename &>(g));
        }

        void exitGroup(typename ImplVisitor<ChildTGroup, ChildTPart>::TraitGroup &g) final {
            visitor.exitGroup(static_cast<typename TGroup::GroupTypename &>(g));
        }

    protected:
        Visitor<ImplVisitor<TGroup, TPart>> &visitor;
    };

}

#pragma once

namespace composite::_impl {

    template<typename TraitGroup, typename TraitPart>
    class VisitorTrait {
    public:
        VisitorTrait() = default;

        virtual void visit(TraitPart &) = 0; //

        virtual bool enterGroup(TraitGroup &) { return true; }; //

        virtual void visitGroup(TraitGroup &) = 0; //
        virtual void exitGroup(TraitGroup &) {}; //

        void _visitChildren(TraitGroup &group) {
            for (auto &it: group._children) {
                it->accept(*this);
            }
        }

        void _onGroup(TraitGroup &group) {
            if (this->enterGroup(group)) {
                // TODO: This is implementing DFS (pre-order) algorithm -- implement others
                this->_visitChildren(group);
                this->visitGroup(group);
                this->exitGroup(group);
            }
        }
    };

}

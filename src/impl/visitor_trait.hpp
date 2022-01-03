#pragma once

namespace composite::_impl {

    template<typename TraitGroupT, typename TraitPartT>
    class VisitorTrait {
    public:
        using TraitGroup = TraitGroupT;
        using TraitPart = TraitPartT;

    public:
        VisitorTrait() = default;

        virtual void visit(TraitPart &) {}

        virtual bool enterGroup(TraitGroup &) { return true; }

        virtual void visitGroup(TraitGroup &) {}

        virtual void exitGroup(TraitGroup &) {}

        void _visitChildren(TraitGroup &group) {
            for (auto &it: group._children) {
                it->accept(*this);
            }
        }

        virtual void _onGroup(TraitGroup &group) = 0;
    };

    template<typename TraitGroup, typename TraitPart>
    class DFSVisitorTrait : public VisitorTrait<TraitGroup, TraitPart> {
    public:
        DFSVisitorTrait() = default;

        void _onGroup(TraitGroup &group) override {
            if (this->enterGroup(group)) {
                this->_visitChildren(group);
                this->visitGroup(group);
                this->exitGroup(group);
            }
        }
    };

    template<typename TraitGroup, typename TraitPart>
    class BFSVisitorTrait : public VisitorTrait<TraitGroup, TraitPart> {
    public:
        BFSVisitorTrait() = default;

        void _onGroup(TraitGroup &group) override {
            if (this->enterGroup(group)) {
                this->visitGroup(group);
                this->_visitChildren(group);
                this->exitGroup(group);
            }
        }
    };


}

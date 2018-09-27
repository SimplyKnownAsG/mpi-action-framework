#include "maf/actions/EndLoopAction.hpp"

namespace maf {
    EndLoopAction::EndLoopAction(bool throw_) : Action(), Exception("ending the loop"), _throw(throw_) {

    };

    EndLoopAction::~EndLoopAction() {

    }

    void EndLoopAction::run() {
        if (this->_throw) {
            auto this_p = shared_from_this();
            throw this_p;
        }
    }

}

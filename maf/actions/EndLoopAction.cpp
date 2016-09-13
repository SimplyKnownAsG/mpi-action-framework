#include "maf/actions/EndLoopAction.hpp"

namespace maf {
    EndLoopAction::EndLoopAction() : Action(), Exception("ending the loop") {

    };

    void EndLoopAction::run() {
        throw this->shared_from_this();
    }

    std::string EndLoopAction::type_name() {
        return "EndLoopAction";
    }

}

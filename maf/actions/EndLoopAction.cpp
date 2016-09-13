#include "maf/actions/EndLoopAction.hpp"

namespace maf {
    EndLoopAction::EndLoopAction() : Action(), Exception("ending the loop") {

    };

    EndLoopAction::~EndLoopAction() {

    }

    void EndLoopAction::run() {
        auto this_p = shared_from_this();
        throw this_p;
    }

    std::string EndLoopAction::type_name() {
        return "EndLoopAction";
    }

}

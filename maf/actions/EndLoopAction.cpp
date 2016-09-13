#include "maf/actions/EndLoopAction.hpp"

#include "maf/Log.hpp"

namespace maf {
    EndLoopAction::EndLoopAction() : Action(), Exception("ending the loop") {
        
    };

    EndLoopAction::~EndLoopAction() {
        
    }

    void EndLoopAction::run() {
        maf::mpi_print("EndLoopAction::run()");
        auto this_p = shared_from_this();
        maf::mpi_print("EndLoopAction::run() about to throw: ", typeid(this_p).name());
        throw this_p;
    }

    std::string EndLoopAction::type_name() {
        return "EndLoopAction";
    }

}

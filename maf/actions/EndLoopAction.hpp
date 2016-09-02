#pragma once

#include "maf/exceptions/Exception.hpp"
#include "maf/actions/Action.hpp"

namespace maf {

    class EndLoopAction : public Action, public Exception {

    public:
        EndLoopAction() : Exception("ending the loop") { };

        void invoke();
    };

}

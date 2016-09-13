#pragma once

#include "maf/exceptions/Exception.hpp"
#include "maf/actions/Action.hpp"

namespace maf {

    class EndLoopAction : public Action, public Exception {

    public:
        EndLoopAction();

        ~EndLoopAction();

        void run() override;

        std::string type_name() override;
    };

}

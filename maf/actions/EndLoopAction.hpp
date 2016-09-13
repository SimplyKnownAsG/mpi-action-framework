#pragma once

#include "maf/exceptions/Exception.hpp"
#include "maf/actions/Action.hpp"

namespace maf {

    class EndLoopAction : public Action, public Exception, public std::enable_shared_from_this<Action> {

    public:
        EndLoopAction();

        void run() override;

        std::string type_name() override;
    };

}

#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"
#include "maf/exceptions/Exception.hpp"

namespace maf {

    class EndLoopAction : public Action, public Exception, public Transmittable<EndLoopAction> {

    public:
        EndLoopAction();

        ~EndLoopAction();

        void run() override;
    };

}

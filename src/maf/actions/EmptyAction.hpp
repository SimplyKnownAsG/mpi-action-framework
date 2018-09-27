#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"

namespace maf {

    class EmptyAction : public Action, public Transmittable<EmptyAction> {

    public:
        EmptyAction();

        ~EmptyAction();

        void run() override;
    };

}

#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"

namespace maf {

    class RevertSplitAction : public Action, public Transmittable<RevertSplitAction> {

    public:
        RevertSplitAction();

        void run() override;

        std::string type_name() override;
    };

}

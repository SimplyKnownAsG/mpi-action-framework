#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class RevertSplitAction : public Action {

    public:
        RevertSplitAction();

        void run() override;

        std::string type_name() override;
    };

}

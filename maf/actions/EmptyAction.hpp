#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class EmptyAction : public Action {

    public:
        EmptyAction();

        ~EmptyAction();

        void run() override;

        std::string type_name() override;
    };

}

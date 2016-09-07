#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class Controller {
    public:

        void start();

        virtual void main() = 0;

        virtual std::shared_ptr<Action> distribute(std::shared_ptr<Action> action=NULL) = 0;

    };

}


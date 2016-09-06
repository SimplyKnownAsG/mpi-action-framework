#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class Controller {
    public:

        void start();

        virtual void main() = 0;

        virtual Action* distribute(Action* action=NULL) = 0;

    };

}


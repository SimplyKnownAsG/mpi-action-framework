#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class Controller {

    public:

        virtual Action *get_action() = 0;

        void start();

        virtual void stop_workers() = 0;

        void start_workers();

    };

}


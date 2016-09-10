#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class Controller {
    public:

        const int rank;

        const int size;

        Controller();

        virtual ~Controller();
        
        void start();

        virtual void main() = 0;

        virtual std::shared_ptr<Action> distribute(std::shared_ptr<Action> action=NULL) = 0;

        virtual std::shared_ptr<Action> scatter(std::vector<std::shared_ptr<Action>> actions) = 0;

    };

}

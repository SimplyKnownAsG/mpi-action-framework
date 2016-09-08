#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class Controller {
    public:

        void start();

        virtual void main() = 0;

#ifdef SWIG
%nodirector;
#endif
        virtual std::shared_ptr<Action> bcast(std::shared_ptr<Action> action=NULL) = 0;
    
    private:

        virtual std::shared_ptr<Action> _default_share(std::shared_ptr<Action> action=NULL) = 0;

    };

}


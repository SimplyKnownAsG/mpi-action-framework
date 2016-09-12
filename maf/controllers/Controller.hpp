#pragma once

#include "maf/actions/Action.hpp"

#include <queue>

namespace maf {

    class Controller : public Action {
    
    protected:

        std::queue<std::shared_ptr<Action>> _queue;

    public:

        const int rank;

        const int size;

        Controller();

        Controller(std::vector<std::shared_ptr<Action>> queue);

        virtual ~Controller();
        
        void start();

        virtual std::shared_ptr<Action> bcast(std::shared_ptr<Action> action=NULL) = 0;

        virtual std::shared_ptr<Action> scatter(std::vector<std::shared_ptr<Action>> actions) = 0;
    
    private:

        virtual std::shared_ptr<Action> _wait() = 0;

        virtual void _stop() = 0;

    };

}

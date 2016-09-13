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

        void start() override;

        virtual void bcast(std::shared_ptr<Action> action=NULL) = 0;

        virtual void bcast(std::vector<std::shared_ptr<Action>> actions) = 0;

        virtual void scatter(std::vector<std::shared_ptr<Action>> actions) = 0;

    private:

        virtual void _wait() = 0;

        virtual void _stop() = 0;

    };

}

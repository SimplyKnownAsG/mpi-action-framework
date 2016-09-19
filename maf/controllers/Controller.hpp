#pragma once

#include "maf/actions/Action.hpp"
#include "maf/Context.hpp"

#include <queue>

namespace maf {

    class Controller : public Action {

    protected:

        std::queue<std::shared_ptr<Action>> _queue;

        void _populate_queue(std::vector<std::shared_ptr<Action>>& queue);

    public:

        int rank;

        int size;

        std::shared_ptr<Context> context;

        Controller();

        Controller(std::vector<std::shared_ptr<Action>> queue);

        virtual ~Controller();

        void start(std::shared_ptr<Context> context = NULL) override;

        virtual void bcast(std::shared_ptr<Action> action = NULL) = 0;

        virtual void bcast(std::vector<std::shared_ptr<Action>> actions) = 0;

        virtual void scatter(std::vector<std::shared_ptr<Action>> actions) = 0;

    private:

        virtual void _wait() = 0;

        virtual void _stop() = 0;

    };

}

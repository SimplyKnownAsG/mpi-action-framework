#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"
#include "maf/Context.hpp"

#include <queue>

namespace maf {

    class Controller : public virtual Action {

    protected:

        std::queue<std::shared_ptr<Action>> _queue;

        void _populate_queue(std::vector<std::shared_ptr<Action>>& queue);

        void _deplete_queue();

    public:

        int rank;

        int size;

        std::shared_ptr<Context> context;

        Controller();

        Controller(std::vector<std::shared_ptr<Action>> queue);

        virtual ~Controller();

        void start(std::shared_ptr<Context> context = NULL) override;

        void start(std::shared_ptr<Action> action);

        void start(std::shared_ptr<Context> context, std::shared_ptr<Action> action);

        void start(std::vector<std::shared_ptr<Action>> actions);

        void start(std::shared_ptr<Context> context, std::vector<std::shared_ptr<Action>> actions);

        virtual void bcast(std::shared_ptr<Action> action = NULL) = 0;

        virtual void bcast(std::vector<std::shared_ptr<Action>> actions) = 0;

        virtual void scatter(std::vector<std::shared_ptr<Action>> actions) = 0;

    private:

        virtual void _wait() = 0;

        virtual void _stop(bool throw_exception = true) = 0;

    };

}

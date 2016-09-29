#pragma once

#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:

        BcastController();

        BcastController(std::vector<std::shared_ptr<Action>> actions);

        void bcast(std::shared_ptr<Action> action = NULL) override;

        void bcast(std::vector<std::shared_ptr<Action>> actions) override;

        void scatter(std::vector<std::shared_ptr<Action>> actions) override;

        virtual void run() override;

        std::string type_name() override;

    private:

        void _wait() override;

        void _stop(bool throw_exception = true) override;

    };

}

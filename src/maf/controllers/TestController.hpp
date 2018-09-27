#pragma once

#include "maf/communication/Transmittable.hpp"
#include "maf/controllers/Controller.hpp"

namespace maf {

    class TestController : public virtual Controller, public virtual Transmittable<TestController> {

    public:

        TestController();

        ~TestController();

        TestController(std::vector<std::shared_ptr<Action>> actions);

        void bcast(std::shared_ptr<Action> action = NULL) override;

        void bcast(std::vector<std::shared_ptr<Action>> actions) override;

        void scatter(std::vector<std::shared_ptr<Action>> actions) override;

        virtual void run() override;

    private:

        void _wait() override;

        void _stop(bool throw_exception = true) override;


    };

}

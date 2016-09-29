#pragma once

#include "maf/controllers/BcastController.hpp"

namespace maf {

    class TestController : public BcastController {

    public:

        TestController();

        virtual void run() override;

        std::string type_name() override;

    };

}

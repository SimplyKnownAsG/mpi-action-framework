#include "mpi.h"

#include "maf/controllers/TestController.hpp"
#include "maf/controllers/BcastController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/actions/ActionFactory.hpp"

#include "maf/Log.hpp"

namespace maf {

    TestController::TestController() : BcastController() {

    }

    void TestController::run() {
        maf::log("Found ", ActionFactory::_Tests().size(), " tests.");
        for (auto test_factory : ActionFactory::_Tests()) {
            auto test = test_factory->create_action();
            this->bcast(test);
        }
        maf::log("Ran ", ActionFactory::_Tests().size(), " tests.");
    }

    std::string TestController::type_name() {
        return "TestController";
    }

}

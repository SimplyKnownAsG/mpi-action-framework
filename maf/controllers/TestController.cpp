#include "mpi.h"

#include "maf/controllers/TestController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/communication/ReadArchive.hpp"
#include "maf/communication/WriteArchive.hpp"
#include "maf/actions/EndLoopAction.hpp"

#include "maf/Log.hpp"

namespace maf {

    TestController::TestController() : Controller() {
        // empty;
    }

    TestController::TestController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    void TestController::bcast(std::shared_ptr<Action> action) {
        std::string data;

        if (action) {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new WriteArchive);
            std::string type_name = action->type_name();
            (*archive) & type_name;
            action->serialize(archive);
            data = archive->str();
        }

        int size = data.size();
        MPI_Bcast(&size, sizeof(size), MPI_INT, 0, MPI_COMM_WORLD);
        data.resize(size);
        MPI_Bcast((void*)data.data(), size * sizeof(char), MPI_CHAR, 0, MPI_COMM_WORLD);

        if (!action) {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new ReadArchive(data));
            action = ActionFactory::Create(archive);
        }

        action->start(this->context);
    }

    void TestController::bcast(std::vector<std::shared_ptr<Action>> actions) {
        std::shared_ptr<Action> controller = std::shared_ptr<Action>(new TestController(actions));
        this->bcast(controller);
    }

    void TestController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        std::shared_ptr<Action> controller = std::shared_ptr<Action>(new ScatterController(actions));
        this->bcast(controller);
    }

    void TestController::run() {
        maf::log("Found ", ActionFactory::_Tests().size(), " tests.");
        for (auto test_factory : ActionFactory::_Tests()) {
            auto test = test_factory->create_action();
            this->bcast(test);
        }
        maf::log("Ran ", ActionFactory::_Tests().size(), " tests.");
    }

    void TestController::_wait() {
        this->bcast();
    }

    void TestController::_stop(bool throw_exception) {
        auto end_act = ActionFactory::Create<EndLoopAction>();
        this->bcast(end_act);

        if (throw_exception) {
            end_act->start(this->context); // throws an EndLoopAction exception to successfully terminate .start()
        }
    }
}

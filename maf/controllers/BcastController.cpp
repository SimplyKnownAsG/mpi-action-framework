#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"

#include "maf/Log.hpp"

namespace maf {

    BcastController::BcastController() : Controller() {
        // empty;
    }

    BcastController::BcastController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    std::shared_ptr<Action> BcastController::bcast(std::shared_ptr<Action> action) {
        if (action) {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new WriteArchive);
            std::string type_name = action->type_name();
            (*archive) & type_name;
            action->serialize(archive);
            archive->bcast();
            return action;
        }
        else {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new ReadArchive);
            archive->bcast();
            std::string type_name;
            (*archive) & type_name;
            std::shared_ptr<Action> result = ActionFactory::Create(type_name);
            result->serialize(archive);
            return result;
        }
    }
    
    std::shared_ptr<Action> BcastController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        maf::mpi_print("BcastController::scatter with ", actions.size(), " actions");
        std::shared_ptr<Action> controller = std::shared_ptr<Action>(new ScatterController(actions));
        this->bcast(controller);
        return controller;
    }

    void BcastController::run() {
        while (!this->_queue.empty()) {
            auto action = this->_queue.front();
            this->_queue.pop();
            this->bcast(action);
            action->run();
        }
        this->_stop();
    }

    std::string BcastController::type_name() {
        return "BcastController";
    }

    std::shared_ptr<Action> BcastController::_wait() {
        return this->bcast();
    }
    
    void BcastController::_stop() {
        auto act = ActionFactory::Create("EndLoopAction");
        this->bcast(act);
        act->run(); // throws an EndLoopAction exception to successfully terminate .start()
    }

}

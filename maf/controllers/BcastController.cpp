#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"

namespace maf {

    BcastController::BcastController() : Controller() {
        // empty;
    }

    BcastController::BcastController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    void BcastController::bcast(std::shared_ptr<Action> action) {
        if (action) {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new WriteArchive);
            std::string type_name = action->type_name();
            (*archive) & type_name;
            action->serialize(archive);
            archive->bcast();
        }
        else {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new ReadArchive);
            archive->bcast();
            action = ActionFactory::Create(archive);
        }

        action->start(this->context);
    }

    void BcastController::bcast(std::vector<std::shared_ptr<Action>> actions) {
        std::shared_ptr<Action> controller = std::shared_ptr<Action>(new BcastController(actions));
        this->bcast(controller);
    }

    void BcastController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        std::shared_ptr<Action> controller = std::shared_ptr<Action>(new ScatterController(actions));
        this->bcast(controller);
    }

    void BcastController::run() {
        while (!this->_queue.empty()) {
            auto action = this->_queue.front();
            this->_queue.pop();
            this->bcast(action);
        }
    }

    std::string BcastController::type_name() {
        return "BcastController";
    }

    void BcastController::_wait() {
        this->bcast();
    }

    void BcastController::_stop() {
        auto act = ActionFactory::Create("EndLoopAction");
        this->bcast(act);
        act->start(this->context); // throws an EndLoopAction exception to successfully terminate .start()
    }

}

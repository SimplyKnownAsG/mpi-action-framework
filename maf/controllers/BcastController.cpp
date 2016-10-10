#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/communication/ReadArchive.hpp"
#include "maf/communication/WriteArchive.hpp"
#include "maf/actions/EndLoopAction.hpp"


namespace maf {

    BcastController::BcastController() : Controller() {
        // empty;
    }

    BcastController::BcastController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    void BcastController::bcast(std::shared_ptr<Action> action) {
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

    void BcastController::_wait() {
        this->bcast();
    }

    void BcastController::_stop(bool throw_exception) {
        auto end_act = std::shared_ptr<Action>(new EndLoopAction(throw_exception));
        this->bcast(end_act);
    }

}

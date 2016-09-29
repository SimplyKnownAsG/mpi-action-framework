#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"

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

    std::string BcastController::type_name() {
        return "BcastController";
    }

    void BcastController::_wait() {
        this->bcast();
    }

    void BcastController::_stop(bool throw_exception) {
        auto end_act = ActionFactory::Create("EndLoopAction");
        this->bcast(end_act);

        if (throw_exception) {
            end_act->start(this->context); // throws an EndLoopAction exception to successfully terminate .start()
        }
    }

}

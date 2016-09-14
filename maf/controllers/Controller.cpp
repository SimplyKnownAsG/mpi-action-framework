
#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/Mpi.hpp"

namespace maf {

    Controller::Controller() : Action(), _queue(), rank(Mpi::GetRank()), size(Mpi::GetSize()) {

    }

    Controller::Controller(std::vector<std::shared_ptr<Action>> queue) : Controller() {
        for (auto action : queue) {
            this->_queue.push(action);
        }
    }

    Controller::~Controller() {
        return;
    }

    void Controller::start() {
        try {
            if (this->rank == 0) {
                this->run();
                this->_stop();
            }
            else {
                while (true) {
                    this->_wait();
                }
            }
        }
        catch (std::shared_ptr<Action>& action) {
            // success
        }
    }

}

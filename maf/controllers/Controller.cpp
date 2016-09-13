
#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/Mpi.hpp"

namespace maf {

    Controller::Controller() : Action(), rank(Mpi::GetRank()), size(Mpi::GetSize()), _queue() {

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
                    std::shared_ptr<Action> action = this->_wait();
                    action->run();
                    action->tear_down();
                }
            }
        }
        catch (std::shared_ptr<Action> action) {
            // success
        }
    }

}

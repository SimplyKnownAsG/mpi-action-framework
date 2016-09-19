
#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/MafComm.hpp"

namespace maf {

    void Controller::_populate_queue(std::vector<std::shared_ptr<Action>>& queue) {
        if (this->rank == 0) {
            for (auto action : queue) {
                this->_queue.push(action);
            }
        }
    }

    Controller::Controller() : Action(), _queue(), rank(MafComm::World->rank), size(MafComm::World->size), context() {

    }

    Controller::Controller(std::vector<std::shared_ptr<Action>> queue) : Controller() {
        this->_populate_queue(queue);
    }

    Controller::~Controller() {
        // empty;
    }

    void Controller::start(std::shared_ptr<Context> context) {
        this->context = context;
        this->set_up();

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

        this->tear_down();
    }

}


#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/communication/MafComm.hpp"

#include "maf/Log.hpp"

namespace maf {

    void Controller::_populate_queue(std::vector<std::shared_ptr<Action>>& queue) {
        if (this->rank == 0) {
            for (auto action : queue) {
                this->_queue.push(action);
            }
        }
    }

    void Controller::_deplete_queue() {
        while (!this->_queue.empty()) {
            this->_queue.pop();
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
                try {
                    this->run();
                }
                catch (...) {
                    this->_stop(false);
                    throw;
                }

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

    void Controller::start(std::shared_ptr<Action> action) {
        this->_queue.push(action);
        this->start();
    }

    void Controller::start(std::shared_ptr<Context> context, std::shared_ptr<Action> action) {
        this->_queue.push(action);
        this->start(context);
    }

    void Controller::start(std::vector<std::shared_ptr<Action>> actions) {
        this->_populate_queue(actions);
        this->start();
    }

    void Controller::start(std::shared_ptr<Context> context, std::vector<std::shared_ptr<Action>> actions) {
        this->_populate_queue(actions);
        this->start(context);
    }

}

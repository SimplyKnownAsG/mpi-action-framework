
#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/Mpi.hpp"

namespace maf {

    Controller::Controller() : rank(Mpi::GetRank()), size(Mpi::GetSize()) {
    }

    Controller::~Controller() {
        return;
    }

    void Controller::start() {
        try {
            if (this->rank == 0) {
                this->main();
                auto act = ActionFactory::Create("EndLoopAction");
                this->_default_share(act);
                act->run();
            }
            else {
                while (true) {
                    std::shared_ptr<Action> action = this->_default_share();
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

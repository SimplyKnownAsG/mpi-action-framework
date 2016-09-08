
#include "maf/controllers/Controller.hpp"
#include "maf/actions/ActionFactory.hpp"
#include <mpi.h>

namespace maf {

    void Controller::start() {
        try {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            if (rank == 0) {
                this->main();
                auto act = ActionFactory::Create("EndLoopAction");
                this->distribute(act);
                act->run();
            }
            else {
                while (true) {
                    std::shared_ptr<Action> action = this->distribute();
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

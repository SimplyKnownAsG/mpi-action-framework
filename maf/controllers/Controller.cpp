
#include "maf/controllers/Controller.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include <mpi.h>

namespace maf {

    void Controller::start() {
        try {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            if (rank == 0) {
                this->main();
                Action *act = new EndLoopAction;
                this->distribute(act);
                act->run(); // strangely deleted below
            }
            else {
                while (true) {
                    Action* action = this->distribute();
                    action->run();
                    delete action;
                }
            }
        }
        catch (EndLoopAction* action) {
            delete action;
        }
    }

}

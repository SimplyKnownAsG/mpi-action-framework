
#include "maf/controllers/Controller.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include <mpi.h>
#include "maf/example.hpp"

namespace maf {

    void Controller::start() {
        try {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            if (rank == 0) {
                this->main();
                auto act = Action::Create("EndLoopAction");
                this->distribute(act);
                act->run(); // strangely deleted below
            }
            else {
                while (true) {
                    std::shared_ptr<Action> action = this->distribute();
                    action->run();
                }
            }
        }
        catch (std::shared_ptr<Action> action) {
            // success
        }
        catch (std::exception &ex) {
            mpi_print("FAILED: &", ex.what());
        }
        catch (std::exception *ex) {
            mpi_print("FAILED: *", ex->what());
        }
        catch (...) {
            mpi_print("FAILED: something \"bad\" happened");
        }
    }

}

#include "mpi.h"

#include "maf/controllers/BcastController.hpp"

namespace maf {

    Action* BcastController::get_action() {
        // MPI_Bcast(&size, sizeof(int), MPI_INT, root, MPI_COMM_WORLD);
        return NULL;
    };

}

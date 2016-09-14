
#include "maf/Log.hpp"
#include "maf/Mpi.hpp"

#include "mpi.h"

namespace maf {

    void log(std::string msg) {
        mpi_print(msg);
    }

    void barrier(std::string msg) {
        int rank = Mpi::GetRank();
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            mpi_print(msg);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

}

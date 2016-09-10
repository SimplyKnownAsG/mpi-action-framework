#include "maf/Mpi.hpp"
#include <mpi.h>

namespace maf {

    int Mpi::GetRank() {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        return rank;
    }

    int Mpi::GetSize() {
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        return size;
    }
}

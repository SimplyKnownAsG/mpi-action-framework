
#include "maf/example.hpp"

namespace maf {

    int Thing::count = 0;

    void mpi_print(std::string msg) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        std::cout << "[" << rank << "] " << msg << std::endl;
    }

}
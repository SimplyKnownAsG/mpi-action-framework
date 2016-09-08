
#include "maf/Log.hpp"

namespace maf {

    int _rank = -100;

    int _get_rank() {
        if (_rank < 0) {
            MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
        }
        return _rank;
    }

    void log(std::string msg) {
        mpi_print(msg);
    }

}
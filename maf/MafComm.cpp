#include "maf/MafComm.hpp"

#include <iostream>

namespace maf {

    std::shared_ptr<MafComm> MafComm::_Init() {
        int initialized;
        MPI_Initialized(&initialized);

        if (!initialized) {
            int argc = 0;
            MPI_Init(&argc, (char***)NULL);
        }

        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        return std::shared_ptr<MafComm>(new MafComm(MPI_COMM_WORLD, rank, size));
    }

    std::shared_ptr<MafComm> MafComm::_World = MafComm::_Init();

    std::shared_ptr<MafComm> MafComm::World = MafComm::_World;

    MPI_Comm MafComm::WorldComm = MafComm::_World->communicator;

    MafComm::MafComm(MPI_Comm communicator, int rank, int size) : communicator(communicator), rank(rank), size(size) {

    }

    void MafComm::take_over_the_world() {
        MafComm::World = this->shared_from_this();
        MafComm::WorldComm = this->communicator;
    }

    void MafComm::barrier() {
        MPI_Barrier(this->communicator);
    }
}

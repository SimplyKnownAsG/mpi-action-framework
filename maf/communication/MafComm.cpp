#include "maf/communication/MafComm.hpp"

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
        return std::shared_ptr<MafComm>(new MafComm(MPI_COMM_WORLD, rank, size, true));
    }

    std::shared_ptr<MafComm> MafComm::_World = MafComm::_Init();

    std::shared_ptr<MafComm> MafComm::World = MafComm::_World;

    MPI_Comm MafComm::WorldComm = MafComm::_World->communicator;

    MafComm::MafComm(MPI_Comm communicator, int rank, int size, bool is_root)
        : communicator(communicator), rank(rank), size(size), is_root(is_root) {

    }

    MafComm::~MafComm() {
        if (this->is_root) {
            MafComm::Finalize();
        }
    }

    void MafComm::Finalize() {
        int finalized;
        MPI_Finalized(&finalized);
        if (!finalized) {
            std::cout << "calling MPI_Finalize()" << std::endl;
            MPI_Finalize();
            std::cout << "calling MPI_Finalize() ... success" << std::endl;
        }
    }

    std::shared_ptr<MafComm> MafComm::split(int color) {
        return this->shared_from_this();
    }

    void MafComm::take_over_the_world() {
        MafComm::World = this->shared_from_this();
        MafComm::WorldComm = this->communicator;
    }

    void MafComm::prepare_for_tomorrow_night() {
        MafComm::World->take_over_the_world();
    }

    void MafComm::abort(int exit_code) {
        MPI_Abort(MafComm::_World->communicator, exit_code);
    }

    void MafComm::barrier() {
        MPI_Barrier(this->communicator);
    }

    template<>
    std::string MafComm::bcast<std::string>(std::string msg, int root) {
        int size = msg.size();
        MPI_Bcast((void*)&size, sizeof(size), MPI_INT, root, this->communicator);

        if (this->rank == root) {
            MPI_Bcast((void*)msg.data(), size, MPI_CHAR, root, this->communicator);
            return msg;
        }
        else {
            std::string buffer = std::string(size, ' ');
            MPI_Bcast((void*)buffer.data(), size, MPI_CHAR, root, this->communicator);
            return buffer;
        }
    }

}

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "mpi.h"

namespace maf {

    class MafComm : public std::enable_shared_from_this<MafComm> {
    private:
        static std::shared_ptr<MafComm> _World;

        MafComm(MPI_Comm communicator, int rank, int size);

        static std::shared_ptr<MafComm> _Init();

    public:

        static std::shared_ptr<MafComm> World;

        static MPI_Comm WorldComm;

        const MPI_Comm communicator;

        const int rank;

        const int size;

        std::shared_ptr<MafComm> split(int color);

        /** Set the World and WorldComm values equal to this MafComm instance.
         *
         * they're pinky and the brain
         * yes, pinky and the brain
         * one is a genius
         * the other's insane.
         * they're laboratory mice
         * their genes have been spliced
         * they're dinky
         * they're pinky and the brain, brain, brain, brain
         * brain, brain, brain, brain
         * brain.
         */
        void take_over_the_world();

        /** Restore the MafComm::World and MafComm::WorldComm equal to actual original world.
         *
         * Brain: We must prepare for tomorrow night.
         * Pinky: Why? What are we going to do tomorrow night?
         * Brain: The same thing we do every night, Pinky - try to take over the world!
         *
         * They're Pinky, They're Pinky and the Brain Brain Brain Brain Brain!
         */
        void prepare_for_tomorrow_night();

        void abort(int exit_code = -1);

        void barrier();

        template<class T>
        T bcast(const T& msg, int root = 0) {
            if (this->rank == root) {
                MPI_Bcast((void*)&msg, sizeof(T), MPI_CHAR, root, this->communicator);
                return msg;
            }
            else {
                T buffer;
                MPI_Bcast((void*)&buffer, sizeof(T), MPI_CHAR, root, this->communicator);
                return buffer;
            }
        };

        // template<class T>
        // std::vector<T> bcast(const std::vector<T>& msg, int root=0) {
        //     int size = msg.size();
        //     MPI_Bcast((void*)&size, sizeof(size), MPI_INT, root, this->communicator);

        //     if (this->rank == root) {
        //         MPI_Bcast((void*)msg.data(), size, MPI_CHAR, root, this->communicator);
        //         return msg;
        //     }
        //     else {
        //         std::vector<T> buffer(size);
        //         MPI_Bcast((void*)buffer.data(), size, MPI_CHAR, root, this->communicator);
        //         return buffer;
        //     }
        // };

    };

    template<>
    std::string MafComm::bcast<std::string>(const std::string& msg, int root);

}

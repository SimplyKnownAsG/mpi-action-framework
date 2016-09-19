#pragma once

#include <memory>
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

        void barrier();
    };

}

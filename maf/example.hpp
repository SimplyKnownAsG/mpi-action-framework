#pragma once

#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>

#include "maf/archives/WriteArchive.hpp"
#include "maf/archives/ReadArchive.hpp"

namespace maf {
    void mpi_print(std::string msg);

    class Thing
    {
    private:
        static int count;
    public:
        int index;
        std::string name;
        Thing(std::string name) : name(name)
        {
            this->index = count++;
        };

        virtual void serialize(Archive *serializer) {
            return;
        };

        void bcast(int root=0) {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            Archive *serializer = NULL;
            if (rank == root) {
                serializer = new WriteArchive();
                this->serialize(serializer);
                serializer->bcast();
            }
            else {
                serializer = new ReadArchive();
                serializer->bcast();
                this->serialize(serializer);
            }
            delete serializer;
        };
    };
}
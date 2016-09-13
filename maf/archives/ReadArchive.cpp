
#include "maf/archives/ReadArchive.hpp"
#include <string>
#include <mpi.h>

namespace maf {

    ReadArchive::ReadArchive() : Archive(), _stream() {

    }

    ReadArchive::ReadArchive(std::string msg) : Archive(), _stream(msg) {

    }

    ReadArchive::ReadArchive(const char* msg, int size) : ReadArchive() {
        std::string s_msg(msg, size);
        this->_stream.str(s_msg);
    }

    void ReadArchive::bcast(int root) {
        int size;
        MPI_Bcast(&size, sizeof(int), MPI_INT, root, MPI_COMM_WORLD);
        char* buffer = new char[size];
        MPI_Bcast((void*)buffer, size * sizeof(char), MPI_CHAR, root, MPI_COMM_WORLD);
        std::string s(buffer, size);
        this->_stream.str(s);
        delete buffer;
    }

    std::string ReadArchive::str() {
        return this->_stream.str();
    }

    size_t ReadArchive::length() {
        return this->_stream.tellg();
    }
}

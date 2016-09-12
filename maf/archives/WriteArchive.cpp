
#include "maf/archives/WriteArchive.hpp"
#include <mpi.h>

namespace maf {

    WriteArchive::WriteArchive() : Archive(), _stream() {

    }

    void WriteArchive::bcast(int root) {
        std::string buffer_as_string = this->_stream.str();
        const char* buffer = buffer_as_string.c_str();
        int size = buffer_as_string.size();
        MPI_Bcast(&size, sizeof(size), MPI_INT, root, MPI_COMM_WORLD);
        MPI_Bcast((void*)buffer, size * sizeof(char), MPI_CHAR, root, MPI_COMM_WORLD);
        // delete buffer; not needed -- buffer_as_string gets deleted when it goes out of scope
    }
    
    std::string WriteArchive::str() {
        return this->_stream.str();
    }
    
    size_t WriteArchive::length() {
        this->_stream.tellp();
    }

}

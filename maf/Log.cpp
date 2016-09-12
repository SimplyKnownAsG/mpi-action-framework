
#include "maf/Log.hpp"

namespace maf {

    void log(std::string msg) {
        mpi_print(msg);
    }

}
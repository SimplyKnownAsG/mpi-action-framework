#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "maf/Mpi.hpp"

namespace maf {

    template<typename T1>
    void flatten(std::ostream& stream, T1 arg1) {
        stream << arg1;
    };

    template<typename T1, typename... TArgs>
    void flatten(std::ostream& stream, T1 arg1, TArgs... args) {
        stream << arg1;
        flatten(stream, args...);
    };

    template<typename... TArgs>
    void mpi_print(TArgs... args) {
        std::ostringstream msg;
        flatten(msg, args...);
        std::cout << "[" << Mpi::GetRank() << "] " << msg.str() << std::endl;
    };

    void log(std::string msg);

}
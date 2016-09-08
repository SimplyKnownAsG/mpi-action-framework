#pragma once

#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>

namespace maf {

    int _get_rank();

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
        std::cout << "[" << _get_rank() << "] " << msg.str() << std::endl;
    };

    void maf_print(std::string msg);

}
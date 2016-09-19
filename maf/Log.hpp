#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "maf/MafComm.hpp"


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
    void log(TArgs... args) {
        std::ostringstream msg;
        flatten(msg, args...);
        std::cout << "[maf::message::" << MafComm::World->rank << "] " << msg.str() << std::endl;
    };

    template<typename... TArgs>
    void warning(TArgs... args) {
        std::ostringstream msg;
        flatten(msg, args...);
        std::cout << "[maf::warning::" << MafComm::World->rank << "] " << msg.str() << std::endl;
    };

    template<typename... TArgs>
    void barrier(TArgs... args) {
        MafComm::World->barrier();

        if (MafComm::World->rank == 0) {
            std::ostringstream msg;
            flatten(msg, args...);
            std::cout << "[maf::barrier::" << MafComm::World->rank << "] " << msg.str() << std::endl;
        }

        MafComm::World->barrier();
    };

}
